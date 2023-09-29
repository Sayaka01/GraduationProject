#pragma once
#include <WinSock2.h>//windows.hより前に記述する

#include "server.h"

//*********************************************************************************************
//  マルチキャスト通信用サーバー（Sender）
//*********************************************************************************************

class CommunicateMultiCastServer :public CommunicateBaseServer
{
public:

    CommunicateMultiCastServer() = default;
    ~CommunicateMultiCastServer()override;

    //UDP通信の初期設定
    bool Initialize(int port)override;
    //ソケットの終了処理
    void Finalize()override;

    //クライアントへUDP通信を使用してデータを送信
    void Send(char* data, int size);

private:
    sockaddr_in localAddress;
    std::string multicast_ip = "224.10.1.1";
};