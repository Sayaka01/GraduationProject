#pragma once
#include <WinSock2.h>//windows.hより前に記述する
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include "../System/console.h"
#include "network_function.h"

//----------------------------------------------------------------------------------------------
//      TCP,UDP通信のクライアント
//----------------------------------------------------------------------------------------------
class CommunicateBaseClient
{
public:
    ////クライアントクラスをインスタンス化
    //static CommunicateClient& Instance()
    //{
    //    static CommunicateClient instance;
    //    return instance;
    //}

    


    CommunicateBaseClient() = default;
    virtual ~CommunicateBaseClient();

    //TCP通信の初期設定
    virtual bool Initialize(int port, char* hostAddress);
    virtual bool Initialize(int port);

    //ソケットの終了処理
    virtual void Finalize();

    void ClearBuffer(){
        memset(buffer, '\0', sizeof(buffer));
    }

    //自分のPCの名前とIPアドレスを取得
    //true:取得に成功した
    sockaddr_in GetHostNameAndAddress(int port);

    //生成などに失敗したときの処理
    // failMessage :生成失敗時に出したいメッセ－ジ
    void Fail(std::string failMessage)
    {
        //エラー内容を表示する
        failMessage += " エラーコード："+std::to_string(WSAGetLastError());
        ConsoleFunc::WriteEndl(failMessage.c_str(),ConsoleFunc::Red);

        completeInitialize = false;
    }

    //成功した場合は1、失敗したらそれ以外(0)
    bool ChangeStringToLong(ADDRESS_FAMILY family, std::string ipStr, ULONG& purposeIp/*変換後のアドレス*/)
    {
        ULONG ip_num = 0;// 変換後IPアドレス
        int pton_result = inet_pton(family, ipStr.c_str(), &ip_num);
        // IPアドレス変換成功時
        if (pton_result == 1)
        {
            purposeIp = ip_num;
            return true;
        }
        else
        {
            //Fail("IPの取得に失敗しました");
            return false;
        }

    }

    //変数のゲッター
    //char GetAction() { return action; }
    char* GetBuffer() { return buffer; }
protected:   
    sockaddr_in ownAddress;
    bool completeInitialize{ false };//初期化が全て正常に実行されたか

    //char action{ (char)-1 };//受信したデータの一番初め(何の行動をするか示している整数を格納)
    char buffer[MAX_BUFFER_SIZE]{};//サーバーから受信したデータを格納

    //ComputerData joinedObjectData[4];//繋がっているコンピューターのデータ
public:
    char inputHostAddress[32]{};

};