#include "Stdafx.h"
#include "client_udp.h"
#include <ws2tcpip.h>

CommunicateUDPClient::~CommunicateUDPClient()
{
}

//void CommunicateUDPClient::InitializeUDPServer(int PortIndex)
//{
//    addrinfo* addrInfo = NULL;
//    addrinfo tmpAddr;
//    ZeroMemory(&tmpAddr, sizeof(addrinfo));
//    ZeroMemory(&clientListUDP, sizeof(clientListUDP));
//    //設定する情報を設定
//    sockaddr_in addrUDP;
//    tmpAddr.ai_flags = 0;
//    tmpAddr.ai_family = AF_INET;
//    tmpAddr.ai_socktype = SOCK_DGRAM;
//    tmpAddr.ai_protocol = 0;
//    addrUDP.sin_family = AF_INET;
//    addrUDP.sin_port = htons(_Port + PortIndex);
//    addrUDP.sin_addr.S_un.S_addr = INADDR_ANY;
//
//    sockaddr_in ownAddr;
//    ownAddr = GetHostNameAndAddress();
//
//    //if (getaddrinfo(inputHostAddress, port, &tmpAddr, &addrInfo) != 0)
//    //{
//    //    Fail("アドレスの取得に失敗しました");
//    //    return;
//    //}
//    //
//    //serverAddr = *((sockaddr_in*)addrInfo->ai_addr);
//
//    //ソケットの生成
//    clientSockUDP = socket(AF_INET, SOCK_DGRAM, 0);
//    if (clientSockUDP == INVALID_SOCKET)
//    {
//        //失敗
//        Fail("ソケットの生成に失敗しました");
//        return;
//    }
//
//    //ソケットと受け入れ情報を関連付ける
//    if (bind(clientSockUDP, (struct sockaddr*)&addrUDP, sizeof(addrUDP)) != 0)
//    {
//        Fail("ソケットと受け入れ情報の関連付けに失敗しました");
//        return;
//    }
//
//    //ノンブロッキングに設定
//    u_long val = 1;
//    ioctlsocket(clientSockUDP, FIONBIO, &val);
//
//    Console::WriteEndl("サーバーが正常に起動しました（UDP）", Console::Blue);
//
//    //clients.data[0].socketUDP = serverSockUDP;
//    //clients.data[0].addrUDP = ownAddr;
//    //std::cout << "ポート番号（UDP）" << clients.data[0].addrUDP.sin_port << std::endl;
//
//    //JoinCommand joinCom;
//    //joinCom.actionType = (char)ActionCommandType::Join;
//    //SendUDP((char*)&joinCom, sizeof(JoinCommand));
//
//    //RecieveUDP();
//}

//UDP通信の初期設定
bool CommunicateUDPClient::Initialize(int port, char* hostAddress)
{
    CommunicateBaseClient::Initialize(port,hostAddress);

    addrinfo* addrInfo = NULL;
    addrinfo tmpAddr;
    ZeroMemory(&tmpAddr, sizeof(addrinfo));
    ZeroMemory(&clientList, sizeof(clientList));
    //設定する情報を設定
    sockaddr_in addrUDP;
    tmpAddr.ai_flags = 0;
    tmpAddr.ai_family = AF_INET;
    tmpAddr.ai_socktype = SOCK_DGRAM;
    tmpAddr.ai_protocol = 0;
    //addrUDP.sin_family = AF_INET;
    //addrUDP.sin_port = htons(_Port + PortIndex);
    //addrUDP.sin_addr.S_un.S_addr = INADDR_ANY;
    std::string portIndex = std::to_string(port);

    //そのアドレスをhostへ送信
    //そこから情報を保存していくようにする

    if (getaddrinfo(inputHostAddress, portIndex.c_str(), &tmpAddr, &addrInfo) != 0)
    {
        Fail("udp:アドレスの取得に失敗しました");
        return false;
    }

    addrUDP = *((sockaddr_in*)addrInfo->ai_addr);

    //ソケットの生成
    ownSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
    if (ownSocket == INVALID_SOCKET)
    {
        //失敗
        Fail("udp:ソケットの生成に失敗しました");
        return false;
    }

    //ソケットと受け入れ情報を関連付ける
    //if (bind(ownSocket, (struct sockaddr*)&addrUDP, sizeof(addrUDP)) != 0)
    //{
    //    Fail("udp:ソケットと受け入れ情報の関連付けに失敗しました");
    //    return;
    //}

    //ノンブロッキングに設定
    u_long val = 1;
    ioctlsocket(ownSocket, FIONBIO, &val);

    ConsoleFunc::WriteEndl("受け入れ情報の関連付けに成功しました。(UDP)", ConsoleFunc::Blue);
    completeInitialize = true;

    //JoinCommand joinCom;
    //joinCom.actionType = (char)ActionCommandType::Join;
    //SendUDP((char*)&joinCom, sizeof(JoinCommand));

    //RecieveUDP();
    return true;
}

//ソケットの
void CommunicateUDPClient::Finalize()
{
    int close = closesocket(ownSocket);
    if (close != 0)
    {
        Fail("終了処理に失敗しました エラーコード：");
    }
}

void CommunicateUDPClient::Send(sockaddr_in recverAddr,char* data, int size)
{
    sendto(ownSocket, data, size, 0, (struct sockaddr*)(&recverAddr), sizeof(sockaddr_in));

}

bool CommunicateUDPClient::Recieve()
{
    //if (!completeInitialize)return false;

    int recieveCount{ 0 };
    int size = sizeof(sockaddr_in);
    sockaddr_in fromAddr;
    recieveCount = recvfrom(ownSocket, (char*)&buffer, sizeof(buffer), 0, (struct sockaddr*)&fromAddr, &size);
    if (recieveCount > 0)
    //if (recv(ownSocket, (char*)&buffer, sizeof(buffer), 0) > 0)
    {
        ConsoleFunc::Write("UDPc:");

        //送られてきたデータをコマンドに変換
        //CommandFromData();
        return true;
    }
    return false;
}

void CommunicateUDPClient::SendChatMessage(char chatMessage[128],sockaddr_in recieverAddr)
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);

    Send(recieverAddr,(char*)&chatCom, sizeof(ChatCommand));
}
