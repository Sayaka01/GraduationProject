#include "Stdafx.h"
#include "client.h"
#include <process.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#include "server.h"

CommunicateBaseClient::~CommunicateBaseClient()
{
    Finalize();
}

bool CommunicateBaseClient::Initialize(int port, char* hostAddress)
{
    //コンソール画面を表示
    ConsoleFunc::OpenWindow();

    //FD_ZERO(&clientList);

    WSADATA data{};
    int value = WSAStartup(MAKEWORD(2, 2), &data);//0が返ってくると問題なく完了
    if (value != 0)
    {
        //失敗
        Fail("初期化に失敗しました");
        return false;
    }
    return true;
}

bool CommunicateBaseClient::Initialize(int port)
{
    //コンソール画面を表示
    ConsoleFunc::OpenWindow();

    //FD_ZERO(&clientList);

    WSADATA data{};
    int value = WSAStartup(MAKEWORD(2, 2), &data);//0が返ってくると問題なく完了
    if (value != 0)
    {
        //失敗
        Fail("初期化に失敗しました");
        return false;
    }
    return true;
}

void CommunicateBaseClient::Finalize()
{
    ConsoleFunc::WriteEndl("接続を終了しました");
    //WSAの開放
    WSACleanup();

    ConsoleFunc::WriteEndl("WSAを開放しました");

}


//自分のPCの名前とIPアドレスを取得
//true:取得に成功した
sockaddr_in CommunicateBaseClient::GetHostNameAndAddress(int port)
{
    sockaddr_in addr;

    //クライアントの受け入れ情報の設定
    addrinfo* addrInfo = NULL;
    addrinfo tmpAddr;
    ZeroMemory(&tmpAddr, sizeof(addrinfo));
    //設定する情報を設定
    tmpAddr.ai_flags = 0;
    tmpAddr.ai_family = AF_INET;
    tmpAddr.ai_socktype = SOCK_DGRAM;
    tmpAddr.ai_protocol = 0;

    std::string p = std::to_string(port);

    char hostname[30];//hostの名前を取得
    if (gethostname(hostname, 30) != 0)
    {
        Fail("ホストの名前の取得に失敗しました");
        return { 0 };
    }

    if (getaddrinfo(hostname, p.c_str(), &tmpAddr, &addrInfo) != 0)
    {
        Fail("ホストの名前からアドレスの取得に失敗しました");
        return { 0 };
    }

    addr = *((sockaddr_in*)addrInfo->ai_addr);

    servent* serv;
    serv = getservbyname(hostname, 0);
    if (serv != nullptr)
    {
        Fail("ポート番号の取得に失敗しました");
    }
    //else
    //{
        //_Port = (int)serv->s_port;
    //}

    std::cout << "ホストのIPAddress(";
    std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b1 << ".";
    std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b2 << ".";
    std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b3 << ".";
    std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;;

    return addr;
}
