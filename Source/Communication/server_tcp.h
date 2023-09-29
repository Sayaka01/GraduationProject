#pragma once
#include <WinSock2.h>//windows.hより前に記述する

#include "server.h"


//*********************************************************************************************
//  ユニキャスト通信用 TCP サーバー
//*********************************************************************************************

class CommunicateTCPServer:public CommunicateBaseServer
{
public:
    CommunicateTCPServer() = default;
    ~CommunicateTCPServer()override;

    // 通信の初期設定
    bool Initialize(int port) override;
    // ソケットの終了処理
    void Finalize()override;

    // select関数を使用してソケットに変化があった時のみクライアントの受け入れ処理
    bool AcceptOnSelect(int emptyIndex, ComputerData* newData);

    // クライアントへTCP通信を使用してデータを送信
    void Send(SOCKET receiverSocket, char* data, int size);
    // クライアントからTCP通信を使用してデータを受信
    bool Recieve(SOCKET senderSocket);
    // select関数を使用してソケットに変化があった時のみクライアントからTCP通信を使用してデータを受信
    bool RecieveOnSelect(SOCKET senderSocket);

    void SetList(SOCKET* sock);
    sockaddr_in& GetSocketAddress() { return addrFromName; }

private:
    sockaddr_in addrFromName;

    fd_set serverList;// ノンブロッキング設定時のサーバーリスト
    fd_set clientList;// ノンブロッキング設定時のクライアントリスト
};
