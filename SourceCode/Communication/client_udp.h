#pragma once
#include <WinSock2.h>//windows.hより前に記述する

#include "client.h"

//*********************************************************************************************
//  ユニキャスト通信用 UDP クライアント
//*********************************************************************************************

class CommunicateUDPClient:public CommunicateBaseClient
{
public:
    CommunicateUDPClient() = default;
    ~CommunicateUDPClient();

    //UDP通信の初期設定
   // void InitializeServer(int PortIndex);
    bool Initialize(int port, char* hostAddress) override;

    //ソケットの終了処理
    void Finalize() override;

    //サーバーへUDP通信を使用してデータを送信
    // data :送りたいデータ　size :送りたいデータのサイズ(構造体のサイズ)
    void Send(sockaddr_in recverAddr,char* data, int size);

    //サーバーからUDP通信を使用してデータを受信
    bool Recieve();

    //チャットメッセージをサーバーへ送信
    // chatMessage :送りたいメッセージ
    void SendChatMessage(char chatMessage[128],sockaddr_in recieverAddr);

private:
    SOCKET ownSocket{};//自分のソケット
    fd_set clientList{};//ノンブロッキング設定時のサーバーリスト
};