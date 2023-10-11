#pragma once
#include <WinSock2.h>//windows.hより前に記述する

#include "server.h"
#include "../System/console.h"

//*********************************************************************************************
//  ユニキャスト通信用 UDP サーバー
//*********************************************************************************************

class CommunicateUDPServer:public CommunicateBaseServer
{
public:

    CommunicateUDPServer() = default;
    ~CommunicateUDPServer()override;

    //UDP通信の初期設定
    bool Initialize(int port)override;
    //ソケットの終了処理
    void Finalize()override;

    //クライアントへUDP通信を使用してデータを送信
    void Send(sockaddr_in receiverAddr, char* data, int size);
    //クライアントからUDP通信を使用してデータを受信
    sockaddr_in Recieve();

    char GetAction() { return action; }
    char* GetBuffer() { return buffer; }

    sockaddr_in& GetSocketAddress() { return addrFromName; }

private:
    sockaddr_in addrFromName;
    fd_set serverList;//ノンブロッキング設定時のサーバーリスト
    fd_set clientList;//ノンブロッキング設定時のクライアントリスト
};
