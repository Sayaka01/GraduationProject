#pragma once

#include <WinSock2.h>//windows.hより前に記述する

#include "client.h"

//*********************************************************************************************
//  ユニキャスト通信用 TCP クライアント
//*********************************************************************************************

class CommunicateTCPClient:public CommunicateBaseClient
{
public:
    CommunicateTCPClient() = default;
    ~CommunicateTCPClient();

    //TCP通信の初期設定
    bool Initialize(int port,char* hostAddress) override;

    //ソケットの終了処理
    void Finalize() override;

    //サーバーへTCP通信を使用してデータを送信
    // data :送りたいデータ　size :送りたいデータのサイズ(構造体のサイズ)
    void Send(char* data, int size);

    //サーバーからTCP通信を使用してデータを受信
    bool Recieve();

    //select関数を使用してソケットに変化があった時のみサーバーからTCP通信を使用してデータを受信
    bool RecieveOnSelect();


    //const ComputerData& GetJoinedData(const int index) { return joinedObjectData[index]; }
private:
    SOCKET ownSocket{};//自分のソケット
    fd_set clientList{};//ノンブロッキング設定時のサーバーリスト

};