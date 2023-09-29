#pragma once
#include <WinSock2.h>//windows.hより前に記述する
#include <Ws2tcpip.h>
#include "client.h"

//*********************************************************************************************
//  マルチキャスト通信用クライアント（Reciever）
//*********************************************************************************************

class CommunicateMultiCastClient :public CommunicateBaseClient
{
public:
    // コンストラクタ
    CommunicateMultiCastClient() = default;
    // デストラクタ
    ~CommunicateMultiCastClient();

    // マルチキャスト通信の初期設定
    bool Initialize(int port) override;


    // 終了処理
    void Finalize() override;

    // データの受信処理
    bool Recieve();

    sockaddr_in& GetFromAddress() { return fromAddr; }

private:
    SOCKET ownSocket{};//自分のソケット
    std::string multicast_ip = "224.10.1.1";//マルチキャストIPアドレス
    // マルチキャストリクエスト
    ip_mreq MulticastRequest;

    //自分のPCの名前とIPアドレスを取得
//true:取得に成功した
    sockaddr_in GetLocalAddress();

    sockaddr_in localAddress;
    //データの送信先を一時的に保存
    sockaddr_in fromAddr;

};
