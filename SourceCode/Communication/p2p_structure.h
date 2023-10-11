#pragma once

#include <WinSock2.h>//windows.hより前に記述する

#include <list>
#include <string>
#include <memory>

//通信システム
#include <d3d11.h>
#include "server_tcp.h"
#include "server_udp.h"
#include "server_multicast.h"
#include "client_tcp.h"
#include "client_udp.h"
#include "client_multicast.h"

//*********************************************************************************************
//  P2P型のホスト、ゲストの処理
//*********************************************************************************************

class PeerToPeerModel
{
public:   
    // コンストラクタ
    PeerToPeerModel()
    {
        WSADATA data;
        //WSAの初期化
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        {
            //初期化失敗
            Fail("WSAの初期化に失敗しました");
            return;
        }
        //データを初期化
        for (int i = 0; i < MaxConnectNumber; i++)
        {
            ClearData(i);
        }

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

        std::string port = std::to_string(61000);

        char hostname[30];//hostの名前を取得
        if (gethostname(hostname, 30) != 0)
        {
            Fail("ホストの名前の取得に失敗しました");
            return ;
        }

        if (getaddrinfo(hostname, port.c_str(), &tmpAddr, &addrInfo) != 0)
        {
            Fail("ホストの名前からアドレスの取得に失敗しました");
            return ;
        }

        addr = *((sockaddr_in*)addrInfo->ai_addr);
        strcat_s(inputHostAddress, std::to_string(addr.sin_addr.S_un.S_un_b.s_b1).c_str());
        strcat_s(inputHostAddress, ".");
        strcat_s(inputHostAddress, std::to_string(addr.sin_addr.S_un.S_un_b.s_b2).c_str());
        strcat_s(inputHostAddress, ".");
        strcat_s(inputHostAddress, std::to_string(addr.sin_addr.S_un.S_un_b.s_b3).c_str());
        strcat_s(inputHostAddress, ".");
        strcat_s(inputHostAddress, std::to_string(addr.sin_addr.S_un.S_un_b.s_b4).c_str());
    }
    // デストラクタ
    virtual ~PeerToPeerModel()
    {
        //終了する際初期値へ戻す
        for (int i = 0; i < MaxConnectNumber; i++)
        {
            ClearData(i);
        }

        //for (auto& mess : recieveMessages)
        //{
        //    delete mess;
        //    mess = nullptr;
        //}
        //recieveMessages.clear();
        //for (auto& rMess : deleteRecieveMessages)
        //{
        //    delete rMess;
        //    rMess = nullptr;
        //}
        //deleteRecieveMessages.clear();

    }

    // 空いているIndexを返す、定員オーバーのときは-1を返す
    int GetEmptyIndex()
    {
        for (int i = 0; i < MaxConnectNumber; i++)
        {
            //登録されていない要素を探し返す
            if (otherGuestData[i].socketTCP == INVALID_SOCKET)
                return i;
        }
        return -1;
    }

    // 初期設定
    virtual bool Initialize(ID3D11Device* device) { return false; }
    
    // ユーザーの名前を登録,初期設定
    virtual bool InitializePassName() { return false; }
   
    // 終了設定
    virtual void Finalize() {}

    //ImGuiを使用したチャットシステム
    void ChatMessageSystemGUI();

    // チャットメッセージをサーバーへ送信
    virtual void SendChatMessage(char chatMessage[128]) 
    {
        ConsoleFunc::WriteEndl("対応していない関数を呼び出しました(SendChatMessage)");
    }
   
    // ゲストの入室待ち
    virtual void WaitConnectFromGuest()
    {
        ConsoleFunc::WriteEndl("対応していない関数を呼び出しました(WaitConnectFromGuest)");
    }
    
    // つなげてきたクライアントの情報を保存
    virtual void RegisterList(int index, ComputerData data) 
    {
        ConsoleFunc::WriteEndl("対応していない関数を呼び出しました(RegisterList)");
    }
    
    // 引数の値(IPアドレス、ポート番号)からすでに登録しているクライアントか調べる
    // 戻り値（既存の場合：登録してあるindex　新規の場合：ー１）
    virtual int CheckAlreadyConnect(sockaddr_in* addr)
    {
        ConsoleFunc::WriteEndl("対応していない関数を呼び出しました(CheckAlreadyConnect)");
        return 10;
    }

    // 送られてきたコマンドを処理する
    virtual void CommandFromData(char* buff) {}

    // データを受け取った時の処理
    virtual void Recieve() {}

    // 繋がっている相手全員にデータを送信
    virtual void SendAll(char* data, int size) {}

    // TCP通信を使用して特定の相手(Indexで指定)にデータを送信
    virtual void SendTCP(char* data, int size,int index =0) {}
    // マルチキャストを使用してデータを全員に送信
    virtual void SendByMulticast(char* data, int size) {}

    // 生成などに失敗したときの処理
    void Fail(std::string failMessage)
    {
        // エラー内容を表示する
        failMessage += " エラーコード：" + std::to_string(WSAGetLastError());
        ConsoleFunc::WriteEndl(failMessage.c_str());
    }

    // コンソール画面に出力
    void WriteDataInConsoleAllTCP()
    {
        for (int i = 0; i < MaxConnectNumber; i++)
        {
            std::cout << "IPAddressリスト" << i << ":(";
            std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
            std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
            std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
            std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
            std::cout << "ポート番号:" << (int)otherGuestData[i].addrTCP.sin_port << std::endl;
        }

    }

    // コンソール画面に出力
    void WriteDataInConsoleIndexTCP(int index)
    {
        std::cout << "IPAddressリスト" << index << ":(";
        std::cout << (int)otherGuestData[index].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
        std::cout << (int)otherGuestData[index].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
        std::cout << (int)otherGuestData[index].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
        std::cout << (int)otherGuestData[index].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
        std::cout << "ポート番号:" << (int)otherGuestData[index].addrTCP.sin_port << std::endl;
    }

    // コンソール画面に出力
    void WriteDataInConsoleIndexUDP(int index)
    {
        std::cout << "IPAddressリスト" << index << ":(";
        std::cout << (int)otherGuestData[index].addrUDP.sin_addr.S_un.S_un_b.s_b1 << ".";
        std::cout << (int)otherGuestData[index].addrUDP.sin_addr.S_un.S_un_b.s_b2 << ".";
        std::cout << (int)otherGuestData[index].addrUDP.sin_addr.S_un.S_un_b.s_b3 << ".";
        std::cout << (int)otherGuestData[index].addrUDP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
        std::cout << "ポート番号:" << (int)otherGuestData[index].addrUDP.sin_port << std::endl;
    }

    // データの初期化
    void ClearData(int index)
    {
        otherGuestData[index].socketTCP = INVALID_SOCKET;
        otherGuestData[index].addrTCP.sin_addr.S_un.S_addr = 0;
        otherGuestData[index].addrTCP.sin_port = 0;
        otherGuestData[index].addrTCP.sin_family = 0;
        otherGuestData[index].addrUDP.sin_addr.S_un.S_addr = 0;
        otherGuestData[index].addrUDP.sin_port = 0;
        otherGuestData[index].addrUDP.sin_family = 0;
        otherGuestData[index].userName = "";
    }

    // 指定のTCPアドレスからUDPアドレスを取得
    sockaddr_in ChangeUDPAddress(sockaddr_in addr);

    // セッターとゲッター
    void SetUserName(std::string name) { playerName = name; }
    void SetPassword(std::string pass) { password = pass; }
    //void SetInitialPos(int i, DirectX::XMFLOAT3 pos) { initialPos[i] = pos; }
    std::string GetPlayerUserName() { return playerName; }
    int GetListIndex() { return listIndex; }
    char GetAction() { return action; }
    char* GetBuffer() { return buffer; }
    char* GetHostAddress() { return inputHostAddress; }
    std::string GetOtherName(int i) { return otherGuestData[i].userName; }

    char inputHostAddress[32]={};

protected:

    struct ChatData
    {
        ChatData(std::string mess, int fr) :message(mess), frame(fr) {}
        std::string message;
        int frame;
    };

    std::list<std::string> messageList;
    char mess[50];
    std::string playerName;
    std::string password;
    int ragTime = 10;
    //std::vector<ChatData*> recieveMessages;
    //std::vector<ChatData*> deleteRecieveMessages;
    //通信してくるクライアントに最大人数
    static const int MaxConnectNumber = 4;
    //0番目:サーバー自身の情報
    ComputerData otherGuestData[MaxConnectNumber]{};//クライアントの情報を格納
    char action;//受信したデータの一番初め(何の行動をするか示している整数を格納)
    char* buffer;
    int listIndex;//自分のIndex番号
};

// ホスト側の処理
class HostP2P:public PeerToPeerModel
{
public:
    HostP2P() = default;
    ~HostP2P()override;

    bool Initialize(ID3D11Device* device) override;
    void Finalize() override;

    // ゲストの入室待ち
    void WaitConnectFromGuest() override;

    // ユーザーの名前を登録,初期設定
    bool InitializePassName() override;

    // チャットメッセージをサーバーへ送信
    void SendChatMessage(char chatMessage[128]) override;

    // つなげてきたクライアントの情報を保存
    void RegisterList(int index, ComputerData data) override;

    // 引数の値(IPアドレス、ポート番号)からすでに登録しているクライアントか調べる
    // 戻り値（既存の場合：登録してあるindex　新規の場合：ー１）
    int CheckAlreadyConnect(sockaddr_in* addr) override;

    void Recieve()override;

    void SendAll(char* data, int size)override;

    // 送られてきたコマンドを処理する
    void CommandFromData(char* buff)override;

    void SendTCP(char* data, int size, int index = 0) override;

    void SendByMulticast(char* data, int size) override;
private:
    CommunicateTCPServer tcpServer;
    CommunicateUDPServer udpServer;
    CommunicateMultiCastServer multicastSender;
    int multiPortIndex = 62000;

    int portIndex = 64000;
};

// ゲスト側の処理
class GuestP2P:public PeerToPeerModel
{
public:
    GuestP2P() = default;
    ~GuestP2P() override;

    bool Initialize(ID3D11Device* device) override;
    void Finalize() override {};

    // チャットメッセージをサーバーへ送信
    void SendChatMessage(char chatMessage[128]) override;
   
    // ユーザーの名前を登録,初期設定
    bool InitializePassName() override;

    // データを受信
    void Recieve()override;

    // 通信中の相手全員に向けてデータを送信
    void SendAll(char* data, int size)override;

    // 送られてきたコマンドを処理する
    void CommandFromData(char* buff)override;

    // TCPを使用してデータを送信
    void SendTCP(char* data, int size, int index = 0) override;
private:
    // TCP用のクライアント
    CommunicateTCPClient tcpClient;
    // UDP用のサーバー
    CommunicateUDPServer udpServer;
    // マルチキャストから受け取り手
    CommunicateMultiCastClient multicastReciever;

    int portIndex = 64000;
    int multiPortIndex = 62000;
};