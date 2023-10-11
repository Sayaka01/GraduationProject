#pragma once

#include "../System/console.h"

#include "server_tcp.h"
#include "server_udp.h"
#include "server_multicast.h"

#include "client_tcp.h"
#include "client_udp.h"
#include "client_multicast.h"



//*********************************************************************************************
//  クラサバ型のホスト、ゲストの処理
//*********************************************************************************************

class ClientServerModel
{
public:
    // 通信に必要なデータ
    struct ConnectData
    {

    };

public:
    // コンストラクタ
    ClientServerModel();
    // デストラクタ
    virtual ~ClientServerModel();


    // 生成などに失敗したときの処理
    void Fail(std::string failMessage)
    {
        // エラー内容を表示する
        failMessage += " エラーコード：" + std::to_string(WSAGetLastError());
        ConsoleFunc::WriteEndl(failMessage.c_str());
    }

    // コンソール画面に通信中の相手のリストを全て表示
    void WriteConsoleConnectListAll();

    // コンソール画面に指定のIPアドレスを表示
    void WriteConsoleIPAdress(sockaddr_in srcAddr);

    // データの初期化
    void ClearData(int index)
    {
        //otherGuestData[index].socketTCP = INVALID_SOCKET;
        //otherGuestData[index].addrTCP.sin_addr.S_un.S_addr = 0;
        //otherGuestData[index].addrTCP.sin_port = 0;
        //otherGuestData[index].addrTCP.sin_family = 0;
        //otherGuestData[index].addrUDP.sin_addr.S_un.S_addr = 0;
        //otherGuestData[index].addrUDP.sin_port = 0;
        //otherGuestData[index].addrUDP.sin_family = 0;
        //otherGuestData[index].userName = "";
    }

    // 空いているIndexを返す、定員オーバーのときは-1を返す
    int GetEmptyIndex()
    {
        for (int i = 0; i < MaxMemberSize; i++)
        {
            //登録されていない要素を探し返す
            if (connectMemberList[i].socketTCP == INVALID_SOCKET)
                return i;
        }
        return -1;
    }

    //初期処理
    virtual bool Initialize();
    
    // 終了処理
    virtual void Finalize();

    // データを送信
    virtual void SendData() {}

    // チャットメッセージを送信
    virtual void SendChatMessage(char chatMessage[128]) {}

    // データを受信
    virtual void RecieveData() {}

    // つなげてきたクライアントの情報を保存
    virtual void RegisterList(int index, ComputerData data) {}

    // 指定のTCPアドレスからUDPアドレスを取得
    sockaddr_in ChangeUDPAddress(sockaddr_in srcAddr);

    // 送られてきたコマンドを処理する
    virtual void ConversionCommandFromData(char* buff) {}

    // 接続待ち
    virtual bool WaitingConnect() { return false; }

    // 引数の値(IPアドレス、ポート番号)からすでに登録しているクライアントか調べる
// 戻り値（既存の場合：登録してあるindex　新規の場合：ー１）
    virtual int CheckAlreadyConnect(sockaddr_in* addr)
    {
        ConsoleFunc::WriteEndl("対応していない関数を呼び出しました(CheckAlreadyConnect)");
        return 10;
    }

public:
    // 接続出来る最大人数
    static const int MaxMemberSize = 4;
    // 通信相手のリスト
    ComputerData connectMemberList[MaxMemberSize]{};

    int appPortIndex = 63000;
    int multiPortIndex = 62000;

    char action;//受信したデータの一番初め(何の行動をするか示している整数を格納)
    char* buffer;
    int ownListIndex{ 0 };

    // マルチキャストサーバー
    CommunicateMultiCastServer* multicastSender{ nullptr };

    // マルチキャストクライアント
    CommunicateMultiCastClient* multicastReciever{ nullptr };
};

class HostCS :public ClientServerModel
{
public:
    // コンストラクタ
    HostCS();
    // デストラクタ
    ~HostCS();

    // 初期処理
    bool Initialize() override;

    // 終了処理
    void Finalize() override;

    // データを送信
    void SendData() override;

    // データを受信
    void RecieveData() override;

    // ゲストの接続待ち
    void Accept();

    // つなげてきたクライアントの情報を保存
    void RegisterList(int index, ComputerData data) override;

    // 引数の値(IPアドレス、ポート番号)からすでに登録しているクライアントか調べる
    // 戻り値（既存の場合：登録してあるindex　新規の場合：ー１）
    int CheckAlreadyConnect(sockaddr_in* addr) override;

    // チャットメッセージを送信
    void SendChatMessage(char chatMessage[128]) override;

    // 送られてきたコマンドを処理する
    void ConversionCommandFromData(char* buff) override;

    // 接続待ち
    bool WaitingConnect() override;

public:
    // TCPサーバー
    CommunicateTCPServer* tcpServer{ nullptr };

    // UDPサーバー
    CommunicateUDPServer* udpServer{ nullptr };


};


class GuestCS :public ClientServerModel
{
public:
    // コンストラクタ
    GuestCS();
    // デストラクタ
    ~GuestCS();

    // 初期処理
    bool Initialize() override;

    //終了処理
    void Finalize() override;

    // データを送信
    void SendData() override;

    // データを受信
    void RecieveData() override;

    // 送られてきたコマンドを処理する
    void ConversionCommandFromData(char* buff) override;

    // 接続待ち
    bool WaitingConnect() override;

    void SendChatMessage(char chatMessage[128])override;

public:
    // TCPクライアント
    CommunicateTCPClient* tcpClient{ nullptr };

    // UDPクライアント
    CommunicateUDPClient* udpClient{ nullptr };

    char hostAddress[32] = {};
};

