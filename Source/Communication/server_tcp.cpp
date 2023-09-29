#include "Stdafx.h"
#include "server_tcp.h"

CommunicateTCPServer::~CommunicateTCPServer()
{
    Finalize();
}

// 通信の初期設定
bool CommunicateTCPServer::Initialize(int port)
{
    CommunicateBaseServer::Initialize(port);

    // クライアントの受け入れ情報の設定
    ownAddress.sin_family = AF_INET;
    ownAddress.sin_port = htons(port);
    ownAddress.sin_addr.S_un.S_addr = INADDR_ANY;

    addrFromName = GetLocalAddress();

    // ソケットの生成
    // TCP:SOCK_STREAM
    // UDP:SOCK_DGRAM
    ownSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ownSocket == INVALID_SOCKET)
    {
        Fail("ソケットの生成に失敗しました");
        return false;
    }

    //ソケットと受け入れ情報を関連付ける
    if (bind(ownSocket, (struct sockaddr*)&ownAddress, sizeof(ownAddress)) != 0)
    {
        Fail("ソケットと受け入れ情報の関連付けに失敗しました");
        return false;
    }

    //ノンブロッキングに設定
    u_long nonBlock = 1;
    ioctlsocket(ownSocket, FIONBIO, &nonBlock);

    //サーバーの接続待ち
    listen(ownSocket, 10);

    ConsoleFunc::WriteEndl("サーバーが正常に起動しました（TCP）", ConsoleFunc::Blue);

    //リストの初期化
    FD_ZERO(&serverList);
    FD_ZERO(&clientList);

    FD_SET(ownSocket, &serverList);

    //clients.data[0].socketTCP = serverSockTCP;
    //clients.data[0].addrTCP = ownAddr;
    //std::cout << "ポート番号（TCP）" << clients.data[0].addrTCP.sin_port << std::endl;

    return true;
}

void CommunicateTCPServer::Finalize()
{
}

//select関数を使用してソケットに変化があった時のみクライアントの受け入れ処理
bool CommunicateTCPServer::AcceptOnSelect(int emptyIndex, ComputerData* newData)
{
    //if (!completeInitialize)return false;

    int len = sizeof(struct sockaddr_in);

    struct timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = 0;
    fd_set socketTmp;
    memcpy(&socketTmp, &serverList, sizeof(serverList));

    //変化があったソケットの数がかえってくる
    int variableSocketCount = 0;
    variableSocketCount = select(0, &socketTmp, NULL, NULL, &tm);
    if (variableSocketCount > 0)
    {
        newData->socketTCP = accept(ownSocket, (struct sockaddr*)&newData->addrTCP, &len);
        //ゲストのソケットの取得に失敗したとき
        if (newData->socketTCP == INVALID_SOCKET)
        {
            int code = WSAGetLastError();
            if (code != WSAEWOULDBLOCK)
            {
                //WSAEWOULDBLOCK=
                // 対象のソケットがノンブロッキングに設定されているため、処理が行えませんでした。
                // 処理を行いたいのであればブロッキングに設定してください。

                Fail("ソケットがノンブロッキングに設定されています。");
            }
        }
        else//ゲストのソケットの接続に成功したとき
        {
            if (emptyIndex < 0)//定員オーバー
            {
                ConsoleFunc::Write("TCP:");
                ConsoleFunc::WriteEndl("定員オーバーです。ごめんなさい");
            }
            else//リストに空きが存在するとき
            {
                return true;
                //クライアントソケットをソケットリストに登録
                //RegisterList(emptyIndex, newData);
            }
        }
    }
    return false;
}

//クライアントへTCP通信を使用してデータを送信
void CommunicateTCPServer::Send(SOCKET receiverSocket, char* data, int size)
{
    send(receiverSocket, data, size, 0);
}

//クライアントからTCP通信を使用してデータを受信
bool CommunicateTCPServer::Recieve(SOCKET senderSocket)
{
    int recieveCount{ 0 };

    recieveCount = recv(senderSocket, (char*)&buffer, sizeof(buffer), 0);

    if (recieveCount > 0)
    {
        ConsoleFunc::Write("TCP:");
        //CommandFromData();
        return true;
    }
    return false;
}

bool CommunicateTCPServer::RecieveOnSelect(SOCKET senderSocket)
{
    //if (!completeInitialize)return false;

    action = {};
    memset(buffer, '\0', sizeof(buffer));
    int recieveCount{ 0 };

    struct timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = 0;
    fd_set socketTmp;
    memcpy(&socketTmp, &clientList, sizeof(clientList));

    //変化があったソケットの数がかえってくる
    int variableSocketCount = 0;
    variableSocketCount = select(0, &socketTmp, NULL, NULL, &tm);
    if (variableSocketCount > 0)
    {
        for (int i = 0; i < 4; i++)
        {
            recieveCount = recv(senderSocket, (char*)&buffer, sizeof(buffer), 0);

            if (recieveCount > 0)
            {
                ConsoleFunc::Write("TCP:");
                //CommandFromData();
                return true;
            }
        }
    }
    return false;}

void CommunicateTCPServer::SetList(SOCKET* sock)
{
    FD_SET(*sock, &clientList);
}
