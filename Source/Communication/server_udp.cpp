#include "Stdafx.h"
#include "server_udp.h"

CommunicateUDPServer::~CommunicateUDPServer()
{
    Finalize();
}

//通信の初期設定
bool CommunicateUDPServer::Initialize(int port)
{
    //UDPサーバーで受け取ったアドレスからクライアントのアドレスを保存しそこに向かって送る

    CommunicateBaseServer::Initialize(port);

    //クライアント受け入れ情報の設定
    ownAddress.sin_family = AF_INET;
    ownAddress.sin_port = htons(port);
    ownAddress.sin_addr.S_un.S_addr = INADDR_ANY;

    addrFromName = GetLocalAddress();

    ownSocket = socket(AF_INET, SOCK_DGRAM, 0);

    //ソケットの生成
    //TCP:SOCK_STREAM
    //UDP:SOCK_DGRAM
    //serverSock = socket(AF_INET, SOCK_STREAM, 0);
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

    ConsoleFunc::WriteEndl("サーバーが正常に起動しました（UDP）", ConsoleFunc::Blue);
    completeInitialize = true;

    return true;
}

void CommunicateUDPServer::Finalize()
{
}

//クライアントへUDP通信を使用してデータを送信
void CommunicateUDPServer::Send(sockaddr_in receiverAddr, char* data, int size)
{
    sendto(ownSocket, data, size, 0, (struct sockaddr*)&receiverAddr, sizeof(sockaddr_in));
}

//クライアントからUDP通信を使用してデータを受信
sockaddr_in CommunicateUDPServer::Recieve()
{
    //if (!completeInitialize)return { 0 };

    int recieveCount{ 0 };
    int size = sizeof(sockaddr_in);
    sockaddr_in fromAddr;//一時的に保存しておくハコ

    recieveCount = recvfrom(ownSocket, (char*)&buffer, sizeof(buffer), 0, (sockaddr*)&fromAddr, &size);
    if (recieveCount > 0)
    {
        ConsoleFunc::Write("UDPs:");
        //if (buffer[0] == (char)ActionCommandType::Join)
        //{
        //    //入出処理
        //    JoinRegisterUDP(fromAddr);
        //    completeInitialize = true;
        //}
        //else
        //{
        

            //その他の更新処理
        //CommandFromData();
        //}
        return fromAddr;
    }
    return { 0 };
}