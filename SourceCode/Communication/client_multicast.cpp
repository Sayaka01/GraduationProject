#include "Stdafx.h"
#include "client_multicast.h"

// デストラクタ
CommunicateMultiCastClient::~CommunicateMultiCastClient()
{
}

// マルチキャスト通信の初期設定
bool CommunicateMultiCastClient::Initialize(int port)
{
    //初期設定
    CommunicateBaseClient::Initialize(port);

    //ソケットの生成
    ownSocket = socket(AF_INET, SOCK_DGRAM, 0);//UDP
    if (ownSocket == INVALID_SOCKET)
    {
        //失敗
        Fail("ソケットの生成に失敗しました");
        return false;
    }


    //ノンブロッキングに設定
    u_long val = 1;
    ioctlsocket(ownSocket, FIONBIO, &val);
    //**************************************************************
    // マルチキャストの設定
    //**************************************************************
    ownAddress.sin_family = AF_INET;
    ownAddress.sin_port = htons(port);
    ownAddress.sin_addr.S_un.S_addr = INADDR_ANY;
//    Address = GetHostNameAndAddress(port);
    localAddress = GetLocalAddress();

    int one = 1;
    setsockopt(ownSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(one));

    // ソケットにアドレス情報をバインド
    int bind_result = bind(ownSocket, (struct sockaddr*)&ownAddress, sizeof(ownAddress));
    // ソケットバインド失敗時のエラー処理
    if (bind_result != 0)
    {
        Fail("マルチキャストのソケットとの関連付けに失敗しました");
        return false;
    }

    // マルチキャストリクエストのセット
    memset(&MulticastRequest, 0, sizeof(MulticastRequest));
    //デフォルトのマルチキャストインターフェースを選択
    MulticastRequest.imr_interface.S_un.S_addr = INADDR_ANY;
    if (!ChangeStringToLong(ownAddress.sin_family, multicast_ip, MulticastRequest.imr_multiaddr.S_un.S_addr))
    {
        Fail("IPアドレスの変換に失敗");
        return false;
    }
    // UDPマルチキャストソケットオプションセット
    int set_opt_result = setsockopt(ownSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&MulticastRequest, sizeof(MulticastRequest));
   
    // ソケットオプションセット失敗時のエラー処理
    if (set_opt_result != 0)
    {        
        Fail("マルチキャストソケットオプションセットに失敗しました");
        return false;
    }

    ConsoleFunc::WriteEndl("マルチキャストクライアントの設定に成功しました。", ConsoleFunc::Blue);
    completeInitialize = true;
    return true;
}

// 終了処理
void CommunicateMultiCastClient::Finalize()
{
    CommunicateBaseClient::Finalize();
}

// データの受信処理
bool CommunicateMultiCastClient::Recieve()
{
    //if (!completeInitialize)return false;

    int size = sizeof(sockaddr_in);

    int recvDataSize = recvfrom(ownSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&fromAddr, &size);
    //データサイズが存在しIPアドレスが一致しない相手からデータを受信したときのみ処理
    if (recvDataSize > 0 /*&& fromAddr.sin_addr.S_un.S_addr != localAddress.sin_addr.S_un.S_addr*/)
    {
        ConsoleFunc::Write("multicast:"); 
        std::cout << (int)buffer[0] << std::endl;
        return true;
    }
    return false;
}

//自分のPCの名前とIPアドレスを取得
//true:取得に成功した
sockaddr_in CommunicateMultiCastClient::GetLocalAddress()
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
