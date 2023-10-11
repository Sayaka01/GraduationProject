#include "Stdafx.h"
#include "server.h"

CommunicateBaseServer::~CommunicateBaseServer()
{
    Finalize();
}

bool CommunicateBaseServer::Initialize(int port)
{
    //コンソール画面を表示
    ConsoleFunc::OpenWindow();

    WSADATA data;
    //WSAの初期化
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        //初期化失敗
        Fail("WSAの初期化に失敗しました");
        return false;
    }
    return true;
}

void CommunicateBaseServer::Finalize()
{
    //ソケットの終了処理
    int close = closesocket(ownSocket);
    if (close != 0)
    {
        Fail("終了処理に失敗しました エラーコード：");
    }

    //WSAの解放
    WSACleanup();
    ConsoleFunc::WriteEndl("WSAを開放しました");
}

//自分のPCの名前とIPアドレスを取得
//true:取得に成功した
sockaddr_in CommunicateBaseServer::GetLocalAddress()
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

    std::string port = std::to_string(ownAddress.sin_port);

    char hostname[30];//hostの名前を取得
    if (gethostname(hostname, 30) != 0)
    {
        Fail("ホストの名前の取得に失敗しました");
        return { 0 };
    }

    if (getaddrinfo(hostname, port.c_str(), &tmpAddr, &addrInfo) != 0)
    {
        Fail("ホストの名前からアドレスの取得に失敗しました");
        return { 0 };
    }

    addr = *((sockaddr_in*)addrInfo->ai_addr);

    //servent* serv;
    //serv = getservbyname(hostname, 0);
    //if (serv != nullptr)
    //{
    //    Fail("ポート番号の取得に失敗しました");
    //}

    //std::cout << "ローカルIPAddress(";
    //std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b1 << ".";
    //std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b2 << ".";
    //std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b3 << ".";
    //std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;;

    return addr;
}

