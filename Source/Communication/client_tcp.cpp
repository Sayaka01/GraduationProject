#include "Stdafx.h"
#include "client_tcp.h"
#include <process.h>
#include <ws2tcpip.h>

CommunicateTCPClient::~CommunicateTCPClient()
{
}

//TCP通信の初期設定
bool CommunicateTCPClient::Initialize(int port, char* hostAddress)
{
    CommunicateBaseClient::Initialize(port,hostAddress);
    //接続に必要な情報を設定
    struct sockaddr_in addr;
    //addr.sin_family = AF_INET;
    //addr.sin_port = htons(port);
    //addr.sin_addr.S_un.S_addr = hostAddress;
    addrinfo hints;
    addrinfo* pAddrInfo = NULL;
    ZeroMemory(&hints, sizeof(addrinfo));

    // 取得する情報を設定
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    std::string portStr = std::to_string(port);
    if (getaddrinfo(hostAddress, (char*)portStr.c_str(), &hints, &pAddrInfo) != 0)
    {
        Fail("アドレスの生成に失敗しました");
        return false;
    }
    addr = *((sockaddr_in*)pAddrInfo->ai_addr);

    //ソケットの生成
    ownSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ownSocket == INVALID_SOCKET)
    {
        //失敗
        Fail("ソケットの生成に失敗しました");
        return false;
    }

    //リストの初期化
    FD_ZERO(&clientList);

    //サーバーへの接続
    int connection = connect(ownSocket, (struct sockaddr*)(&addr), sizeof(addr));
    if (connection != 0)
    {
        Fail("接続に失敗しました");
        return false;
    }
    FD_SET(ownSocket, &clientList);

    ConsoleFunc::WriteEndl("サーバーに接続しました。(TCP)", ConsoleFunc::Blue);
    //CommunicateCommand com;
    //com.actionType = static_cast<int>(ActionCommandType::Exit);
    //UpdateCommand upCom{};
    //upCom.actionType = static_cast<char>(ActionCommandType::Update);
    //upCom.personType = static_cast<char>(PersonType::Player);
    //upCom.pos = { 1.5f,6.4f,3.3f };
    //strcpy_s(info.name, sizeof(playerName), playerName);
    //JoinCommand joinCom;
    //joinCom.actionType = (char)ActionCommandType::Join;
    //SendTCP((char*)&joinCom,sizeof(JoinCommand));

    struct sockaddr_in localAddress {};
    socklen_t addressLength = sizeof(localAddress);
    if (getsockname(ownSocket, reinterpret_cast<struct sockaddr*>(&localAddress), &addressLength) < 0) {
        std::cout << "ソケット名の取得中にエラーが発生しました" << std::endl;
        //closesocket(ownSocket);
    }


    char ipBuffer[INET_ADDRSTRLEN];
    const char* ipAddress = inet_ntop(AF_INET, &(localAddress.sin_addr), ipBuffer, INET_ADDRSTRLEN);
    if (ipAddress == nullptr) {
        std::cout << "IPアドレスの変換中にエラーが発生しました" << std::endl;
    }

    printf("%d.%d.%d.%d",
        localAddress.sin_addr.S_un.S_un_b.s_b1,
        localAddress.sin_addr.S_un.S_un_b.s_b2,
        localAddress.sin_addr.S_un.S_un_b.s_b3,
        localAddress.sin_addr.S_un.S_un_b.s_b4);


    completeInitialize = true;
    return true;
}

void CommunicateTCPClient::Finalize()
{
    int close = closesocket(ownSocket);
    if (close != 0)
    {
        Fail("終了処理に失敗しました エラーコード：");
    }
}

//サーバーへTCP通信を使用してデータを送信
void CommunicateTCPClient::Send(char* data, int size)
{
    send(ownSocket, data, size, 0);
}

//サーバーからTCP通信を使用してデータを受信
bool CommunicateTCPClient::Recieve()
{
    //if (!completeInitialize)return false;

    if (recv(ownSocket, (char*)&buffer, sizeof(buffer), 0) > 0)
    {
        ConsoleFunc::Write("TCP:");
        //送られてきたデータをコマンドに変換
        //CommandFromData();
        return true;
    }
    return false;
}

//select関数を使用してソケットに変化があった時のみサーバーからTCP通信を使用してデータを受信
bool CommunicateTCPClient::RecieveOnSelect()
{
    //if (!completeInitialize)return false;

    //action = {};
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
        recieveCount = recv(ownSocket, (char*)&buffer, sizeof(buffer), 0);

        if (recieveCount > 0)
        {
            ConsoleFunc::Write("TCP:");
            //送られてきたデータをコマンドに変換
            //CommandFromData();
            return true;
        }
    }
    return false;
}
