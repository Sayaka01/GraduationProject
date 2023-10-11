#include "Stdafx.h"
#include "client_server_structure.h"

#include "../System/console.h"
//#include <magic_enum.hpp>

//-------------------------------------------------------------------------------------------------------------
// クライアントサーバー型　
//-------------------------------------------------------------------------------------------------------------

// コンストラクタ
ClientServerModel::ClientServerModel()
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
    for (int i = 0; i < MaxMemberSize; i++)
    {
        ClearData(i);
    }


    ConsoleFunc::OpenWindow();
}

// デストラクタ
ClientServerModel::~ClientServerModel()
{
    if (multicastSender != nullptr)
        delete multicastSender;
    if (multicastReciever != nullptr)
        delete multicastReciever;
    for (int i = 0; i < MaxMemberSize; i++)
    {
        int close = closesocket(connectMemberList[i].socketTCP);
        if (close != 0)
        {
            Fail("終了処理に失敗しました エラーコード：");
        }
    }
    //WSAの解放
    WSACleanup();

}

// コンソール画面に通信中の相手のリストを全て表示
void ClientServerModel::WriteConsoleConnectListAll()
{
    for (int i = 0; i < MaxMemberSize; i++)
    {
        std::cout << "IPAddressリスト" << i << ":(";
        //std::cout << (int)connectMemberList[i].sin_addr.S_un.S_un_b.s_b1 << ".";
        //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
        //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
        //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
        //std::cout << "ポート番号:" << (int)connectMemberList[i].addrTCP.sin_port << std::endl;
    }
}

// コンソール画面に指定のIPアドレスを表示
void ClientServerModel::WriteConsoleIPAdress(sockaddr_in srcAddr)
{
    ConsoleFunc::Write("IPAddress : ");
    ConsoleFunc::WriteEndl((
          std::to_string((int)srcAddr.sin_addr.S_un.S_un_b.s_b1) + "."
        + std::to_string((int)srcAddr.sin_addr.S_un.S_un_b.s_b2) + "."
        + std::to_string((int)srcAddr.sin_addr.S_un.S_un_b.s_b3) + "."
        + std::to_string((int)srcAddr.sin_addr.S_un.S_un_b.s_b4)
        ).c_str());
    ConsoleFunc::WriteEndl(("ポート番号:" + std::to_string((int)srcAddr.sin_port)).c_str());
}

//初期処理
bool ClientServerModel::Initialize()
{
    return false;
}

// 終了処理
void ClientServerModel::Finalize()
{
}

// 指定のTCPアドレスからUDPアドレスを取得
sockaddr_in ClientServerModel::ChangeUDPAddress(sockaddr_in srcAddr)
{
    sockaddr_in resultAddr;

    // クライアントの受け入れ情報の設定
    addrinfo* addrInfo = NULL;
    addrinfo tmpAddr;
    ZeroMemory(&tmpAddr, sizeof(addrinfo));
    // 設定する情報を設定
    tmpAddr.ai_flags = 0;
    tmpAddr.ai_family = AF_INET;
    tmpAddr.ai_socktype = SOCK_DGRAM;
    tmpAddr.ai_protocol = 0;
    // アドレスの型変更
    std::string strAddress =
        std::to_string(srcAddr.sin_addr.S_un.S_un_b.s_b1) + "." +
        std::to_string(srcAddr.sin_addr.S_un.S_un_b.s_b2) + "." +
        std::to_string(srcAddr.sin_addr.S_un.S_un_b.s_b3) + "." +
        std::to_string(srcAddr.sin_addr.S_un.S_un_b.s_b4);
    std::string strPort = std::to_string(srcAddr.sin_port);

    if (getaddrinfo(strAddress.c_str(), strPort.c_str(), &tmpAddr, &addrInfo) != 0)
    {
        Fail("ホストの名前からアドレスの取得に失敗しました");
        return { 0 };
    }
    resultAddr = *((sockaddr_in*)addrInfo->ai_addr);

    //std::cout << "ホストの";
    WriteConsoleIPAdress(resultAddr);

    return resultAddr;
}
//-------------------------------------------------------------------------------------------------------------
// クライアントサーバー型　ホスト
//-------------------------------------------------------------------------------------------------------------

// コンストラクタ
HostCS::HostCS()
{
}

// デストラクタ
HostCS::~HostCS()
{
    if (tcpServer != nullptr)
        delete tcpServer;
    if (udpServer != nullptr)
        delete udpServer;
}

// 初期処理
bool HostCS::Initialize()
{
    ClientServerModel::Initialize();

    //それぞれのサーバーを起動
    tcpServer = new CommunicateTCPServer();
    udpServer = new CommunicateUDPServer();
    multicastSender = new CommunicateMultiCastServer();
    multicastReciever = new CommunicateMultiCastClient();


    ownListIndex = 0;

    //それぞれのサーバーを起動
    if (tcpServer->Initialize(appPortIndex) == false)
        return false;

    if (udpServer->Initialize(appPortIndex) == false)
        return false;

    // マルチキャスト用サーバー起動
    if (multicastSender->Initialize(multiPortIndex) == false)
        return false;


    // マルチキャスト用クライアント起動
    if (multicastReciever->Initialize(multiPortIndex) == false)
        return false;

    // ホスト自身の情報を保存
    connectMemberList[0].socketTCP = tcpServer->GetSocket();
    connectMemberList[0].addrTCP = tcpServer->GetSocketAddress();
    connectMemberList[0].addrUDP = ChangeUDPAddress(tcpServer->GetSocketAddress());
    return true;

}

// 終了処理
void HostCS::Finalize()
{
    // TCPの接続を切断

    // UDPの接続を切断

    // マルチキャストの接続を切断


}


// データを送信
void HostCS::SendData()
{
}

// チャットメッセージを送信
void HostCS::SendChatMessage(char chatMessage[128])
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //chatCom.actionFrame = SceneManager::Instance().GetActionFrame() + ragTime;// 1秒後(10フレーム後)に表示
    //snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage)+1, chatMessage);

    for (int i = 1; i < MaxMemberSize; i++)
    {
        if (connectMemberList[i].socketTCP != INVALID_SOCKET)
            tcpServer->Send(connectMemberList[i].socketTCP, (char*)&chatCom, sizeof(ChatCommand));
    }
}

// 接続待ち
bool HostCS::WaitingConnect()
{
    int connectIndex = GetEmptyIndex();

    if (multicastReciever->Recieve())
    {
        //参加許可をうけとったらTCPとUDPの準備をする
        buffer = multicastReciever->GetBuffer();
        action = buffer[0];

        //参加許可の場合
        if (action == static_cast<int>(ActionCommandType::Join))
        {
            ConsoleFunc::Write("ゲストが見つかりました");

            // 通信中の人数が最大数を超えた場合は参加拒否を出す
            if (connectIndex < 0)
            {
                // 参加してきたものに対して退出を出す
                ExitCommand exitCom;
                exitCom.actionType = (char)ActionCommandType::Exit;
                exitCom.index = -1;
                multicastSender->Send((char*)&exitCom, sizeof(ExitCommand));
            }
            else //人数が最大数を超えていない場合
            {
                // 参加してきたものに対して参加許可を出す
                JoinPermitCommand joinPerCom;
                joinPerCom.actionType = (char)ActionCommandType::JoinPermit;
                joinPerCom.index = connectIndex;
                for (int i = 0; i < MaxMemberSize; i++)
                {
                    //ソケット情報を持っている場合のみメンバーのリストをコピー
                    if (connectMemberList[i].socketTCP == INVALID_SOCKET)continue;
                    
                    joinPerCom.memberList[i] = connectMemberList[i];
                }
                multicastSender->Send((char*)&joinPerCom, sizeof(JoinPermitCommand));
            }
         
        }
    }

    ComputerData data{};

    // TCP接続待ち
    if (tcpServer->AcceptOnSelect(appPortIndex, &data))
    {
        // ゲストのデータを保管
        RegisterList(connectIndex, data);
        tcpServer->SetList(&connectMemberList[connectIndex].socketTCP);
        char mess[] = "nice to meet you.";
        SendChatMessage(mess);
    }

    return false;
}

// データを受信
void HostCS::RecieveData()
{
    // マルチキャスト通信で受信
    if (multicastReciever->Recieve())
    {
        // 受信したデータからコマンドを取得
        ConversionCommandFromData(udpServer->GetBuffer());
    }


    buffer = nullptr;
    action = -1;

    // UDP通信で受信
    if (udpServer->Recieve().sin_addr.S_un.S_addr != 0)// 受信に成功した場合
    {
        // 受信したデータからコマンドを取得
        ConversionCommandFromData(udpServer->GetBuffer());

        // 送信者の情報を受け取った場合は新規登録と既存の通信者に送信者の情報を送る
        //if (action == (char)ActionCommandType::GiveOwnData)
        //{
        //    OwnDataCommand* ownDataCom = (OwnDataCommand*)(udpServer.GetBuffer());
        //    int index = ownDataCom->guestIndex;
        //    otherGuestData[index].addrUDP = udpAddr;
        //    otherGuestData[index].userName = ownDataCom->name;
        //    // パスワードが違う場合　　　
        //    if (password != ownDataCom->password)
        //    {
        //        // 退出コマンドを送信
        //        ExitCommand exitCom;
        //        exitCom.playerIndex = index;
        //        tcpServer.Send(otherGuestData[index].socketTCP, (char*)&exitCom, sizeof(ExitCommand));

        //        // 相手のデータを削除
        //        ClearData(index);
        //    }
        //    else
        //    {                
        //        ChatCommand chatCom;
        //        chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
        //        char chatMessage[128] = { "your udp address add." };
        //        strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);
        //        // 情報を登録したことを相手に伝える
        //        udpServer.Send(otherGuestData[index].addrUDP, (char*)&chatCom, sizeof(ChatCommand));

        //        // 送信者以外に送信者の入出を知らせる
        //        for (int i = 1; i < MaxConnectNumber; i++)
        //        {
        //            if (i == index)continue;
        //            if (otherGuestData[i].socketTCP == INVALID_SOCKET)continue;
        //            // 既存ゲストに新規ゲストの増加を送信
        //            IncreaseGuestCommand increaseGuestCom;
        //            increaseGuestCom.actionType = (char)ActionCommandType::IncreaseGuest;
        //            increaseGuestCom.index = index;
        //            increaseGuestCom.newGuestData = otherGuestData[index];
        //            tcpServer.Send(otherGuestData[i].socketTCP, (char*)&increaseGuestCom, sizeof(IncreaseGuestCommand));
        //        }
        //    }
        //}

    }

    // TCP通信でソケットに変化があった場合のみ受信
    for (int i = 0; i < MaxMemberSize; i++)
    {
        //通信中の相手の情報があるときだけ受信
        if (connectMemberList[i].socketTCP == INVALID_SOCKET)continue;
        if (tcpServer->Recieve(connectMemberList[i].socketTCP))
        {
            // 受信したデータからコマンドを取得
            ConversionCommandFromData(tcpServer->GetBuffer());
        }
    }

}

// ゲストの接続待ち
void HostCS::Accept()
{
}

// つなげてきたクライアントの情報を保存
void HostCS::RegisterList(int index, ComputerData data)
{
    // 既に通信済かどうか確認する
    int connectIndex = CheckAlreadyConnect(&data.addrTCP);

    if (connectIndex >= 0)// 既に通信済の相手であれば登録しない
    {
        ConsoleFunc::WriteEndl("既に入出済です");
        return;
    }

    // 新規の相手の場合
    // 新規登録をする
    connectMemberList[index] = data;

    ConsoleFunc::WriteEndl(("新規クライアントを登録しました" + std::to_string(index)).c_str());

    // 参加許可コマンドを送信
    //JoinPermitCommand joinPermitCom;
    //joinPermitCom.actionType = (char)ActionCommandType::JoinPermit;
    //joinPermitCom.index = index;
    //tcpServer->Send(connectMemberList[index].socketTCP, (char*)&joinPermitCom, sizeof(JoinPermitCommand));

    // TCPのアドレスからUDPのアドレスへ変換
    connectMemberList[index].addrUDP = ChangeUDPAddress(connectMemberList[index].addrTCP);

    // つなげてきたプレイヤーのモデルを追加
    //SceneManager::Instance().JoinPlayer(index, initialPos[index]);

    // 通信している人たちの名前のリストを送る
    //PassNameCommand passName;
    //passName.actionType = static_cast<char>(ActionCommandType::PassName);
    //for (int i = 0; i < MaxConnectNumber; i++)
    //{
    //    char name[20]{};
    //    strcpy_s(name, strlen(otherGuestData[i].userName.c_str()) + 1, otherGuestData[i].userName.c_str());
    //    passName.name[i] = std::string(name);
    //}
    //tcpServer.Send(otherGuestData[index].socketTCP, (char*)&passName, sizeof(PassNameCommand));
}

// 引数の値(IPアドレス、ポート番号)からすでに登録しているクライアントか調べる
// 戻り値（既存の場合：登録してあるindex　新規の場合：ー１）
int HostCS::CheckAlreadyConnect(sockaddr_in* addr)
{
    for (int i = 0; i < MaxMemberSize; i++)
    {
        // アドレスとポート番号が一致するか調べる
        if (addr->sin_addr.S_un.S_addr == connectMemberList[i].addrUDP.sin_addr.S_un.S_addr &&
            addr->sin_port == connectMemberList[i].addrUDP.sin_port)
            return i;
    }
    return -1;
}

// 送られてきたコマンドを処理する
void HostCS::ConversionCommandFromData(char* buff)
{
    buffer = buff;
    action = buffer[0];
    // 送られてきたコマンドに対応する処理をする
    switch (action)
    {
    case static_cast<int>(ActionCommandType::Exit):// 退出要請
        ConsoleFunc::Write("退出コマンド(index: ");
        {
            //ExitCommand* recieveExitCom = (ExitCommand*)(buff);
            // 退出コマンドを送ってきたPCのデータが存在していれば
            //if (otherGuestData[recieveExitCom->playerIndex].socketTCP != INVALID_SOCKET)
            //{
            //    ConsoleFunc::Write((std::to_string(recieveExitCom->playerIndex)+" ):").c_str());
            //    // 登録データを削除
            //    ClearData(recieveExitCom->playerIndex);
            //    SceneManager::Instance().ExitPlayer(recieveExitCom->playerIndex);

            //    // 送ってきたゲストに退出許可コマンドを送る
            //    ExitCommand exitPermitCom{};
            //    exitPermitCom.actionType = (char)ActionCommandType::ExitPermit;
            //    exitPermitCom.playerIndex = recieveExitCom->playerIndex;
            //    SendTCP((char*)&exitPermitCom, sizeof(ExitCommand), exitPermitCom.playerIndex);
            //    
            //    // 送ってきたゲストと他の接続しているゲストに退出したゲストがいることを知らせる
            //    ExitCommand exitNoticeCom{};
            //    exitNoticeCom.actionType = (char)ActionCommandType::ExitNotice;
            //    exitNoticeCom.playerIndex = recieveExitCom->playerIndex;

            //    for (int i = 1; i < MaxConnectNumber; i++)
            //    {
            //        if (otherGuestData[i].socketTCP == INVALID_SOCKET)continue;
            //        SendTCP((char*)&exitNoticeCom, sizeof(ExitCommand), i);
            //    }
            //    ConsoleFunc::WriteEndl("退出処理正常終了");
            //}
            ConsoleFunc::WriteEndl("既に退出済のPCからの実行");
        }
        break;
    case static_cast<int>(ActionCommandType::Chat):// チャット受信
        {
            ConsoleFunc::WriteEndl("チャット受信");
            ChatCommand* chatCom = (ChatCommand*)(buff);
            ConsoleFunc::Write(chatCom->message);
            //ConsoleFunc::WriteEndl(std::to_string(chatCom->actionFrame).c_str());
            //ChatData* chatData = new ChatData(chatCom->message, chatCom->actionFrame);
            //recieveMessages.emplace_back(chatData);
        }
        break;
    default:
        ConsoleFunc::Write("受信したコマンド：");
        //ConsoleFunc::WriteEndl(magic_enum::enum_name(static_cast<ActionCommandType>(action)).data());
        break;
    }
    std::cout << std::endl;
}



//-------------------------------------------------------------------------------------------------------------
// クライアントサーバー型　ゲスト
//-------------------------------------------------------------------------------------------------------------

// コンストラクタ
GuestCS::GuestCS()
{
}

// デストラクタ
GuestCS::~GuestCS()
{
    if (tcpClient != nullptr)
        delete tcpClient;
    if (udpClient != nullptr)
        delete udpClient;
}

// 初期処理
bool GuestCS::Initialize()
{
    //それぞれのクライアントを起動
    tcpClient = new CommunicateTCPClient();
    udpClient = new CommunicateUDPClient();
    multicastSender = new CommunicateMultiCastServer();
    multicastReciever = new CommunicateMultiCastClient();

    // クライアントを起動

    // TCPサーバーへ接続

    // UDPサーバーへ接続

    // マルチキャストに登録

    ClientServerModel::Initialize();

    ownListIndex = 1;

    // マルチキャスト用サーバー起動
    if (multicastSender->Initialize(multiPortIndex) == false)
        return false;

    // マルチキャスト用クライアント起動
    if (!multicastReciever->Initialize(multiPortIndex))
        return false;

    //マルチキャストへの登録が完了したら参加コマンドをホストへ送信する
    // 参加コマンドを送信
    int roomindex = 2;
    int password = 111;
    JoinCommand joinCom;
    joinCom.actionType = (char)ActionCommandType::Join;
    joinCom.roomIndex = roomindex;
    joinCom.password = password;
    multicastSender->Send((char*)&joinCom, sizeof(JoinCommand));


    // TCPクライアントを起動
    //if (tcpClient->Initialize(appPortIndex, hostAddress) == false)
    //    return false;

    // 入出許可を受信する
    //if (tcpClient->Recieve())
    //    ConversionCommandFromData(tcpClient->GetBuffer());

    // UDPサーバーの起動
    //if (udpClient->Initialize(appPortIndex + ownListIndex, hostAddress) == false)
    //    return false;


    // 名前の情報を受け取る
    //if (tcpClient->Recieve())
    //    ConversionCommandFromData(tcpClient->GetBuffer());

    return true;

}

//終了処理
void GuestCS::Finalize()
{
    // TCPサーバーへの接続を切断
    
    // UDPサーバーへの接続を切断
    
    // マルチキャストサーバーへの接続を切断

}

// データを送信
void GuestCS::SendData()
{
}

// データを受信
void GuestCS::RecieveData()
{
    // データを受信
    buffer = nullptr;
    action = -1;
    if (udpClient->Recieve())
    { 
        // 受信したデータからコマンドへ変換
        ConversionCommandFromData(udpClient->GetBuffer());
    }
    if (multicastReciever->Recieve())
    {
        // 受信したデータからコマンドへ変換
        ConversionCommandFromData(multicastReciever->GetBuffer());
    }
    if (tcpClient->RecieveOnSelect())
    {
        // 受信したデータからコマンドへ変換
        ConversionCommandFromData(tcpClient->GetBuffer());
    }
}

// 送られてきたコマンドを処理する
void GuestCS::ConversionCommandFromData(char* buff)
{
    buffer = buff;
    action = buff[0];

    switch (action)
    {
    case static_cast<int>(ActionCommandType::JoinPermit):// 入室許可
        ConsoleFunc::WriteEndl("入出許可を受信");
        {
            JoinPermitCommand* joinPermitCom = (JoinPermitCommand*)(buff);
            ownListIndex = (int)joinPermitCom->index;
            //SceneManager::Instance().SetHostStartActionFrame(joinPermitCom->actionFrame);
            std::cout << "自分の識別番号:" << ownListIndex << std::endl;
            for (int i = 0; i < MaxMemberSize; i++)
            {
                if (joinPermitCom->memberList[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                connectMemberList[i] = joinPermitCom->memberList[i];
                std::cout << "つながっているPCのIPAddress(";
                std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
                std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
                std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
                std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
                std::cout << "ポート番号:" << (int)connectMemberList[i].addrTCP.sin_port << std::endl;
                // プレイヤーの参加情報をゲームシーンで処理
                //if ((int)joinPermitCom->ownIndex != i)
                //    SceneManager::Instance().JoinPlayer(i, TypeConversionShort3::ChangeFloat3Decimal2(joinPermitCom->joinPosition[i]));
            }
        }
        break;
    case static_cast<int>(ActionCommandType::Exit):// 退出要請
        ConsoleFunc::WriteEndl("退出要請を受信");
        break;
    case static_cast<int>(ActionCommandType::ExitPermit):// 退出許可
        ConsoleFunc::WriteEndl("退出許可を受信");
        break;
    case static_cast<int>(ActionCommandType::Chat):// チャット受信  
        {
            ConsoleFunc::WriteEndl("チャット受信");
            ChatCommand* chatCom = (ChatCommand*)(tcpClient->GetBuffer());
            ConsoleFunc::WriteEndl(chatCom->message);
            //ConsoleFunc::WriteEndl(std::to_string(chatCom->actionFrame).c_str());
            //ChatData* chatData = new ChatData(chatCom->message, chatCom->actionFrame);
            //recieveMessages.emplace_back(chatData);
        }
    break;
    default:
    {
        ActionCommandType state = static_cast<ActionCommandType>(action);
        ConsoleFunc::Write("コマンド：");
        //ConsoleFunc::WriteEndl(magic_enum::enum_name(state).data());
    }
    break;
    }
    std::cout << std::endl;
}

bool GuestCS::WaitingConnect()
{
    //参加許可をうけとったらTCPとUDPの準備をする
    if (multicastReciever->Recieve())
    {
        buffer = multicastReciever->GetBuffer();
        action = buffer[0];

        //参加許可の場合
        if (action == static_cast<int>(ActionCommandType::JoinPermit))
        {
            ConsoleFunc::WriteEndl("ホストが見つかりました");

            //送信者のIPアドレスを取得
            sockaddr_in addr = multicastReciever->GetFromAddress();
            std::string addrStr=
                std::to_string(addr.sin_addr.S_un.S_un_b.s_b1) + "." +
                std::to_string(addr.sin_addr.S_un.S_un_b.s_b2) + "." +
                std::to_string(addr.sin_addr.S_un.S_un_b.s_b3) + "." +
                std::to_string(addr.sin_addr.S_un.S_un_b.s_b4);
            strcpy_s(hostAddress, strlen(addrStr.c_str()) + 1, addrStr.c_str());

            ConsoleFunc::Write("ホストの");
            WriteConsoleIPAdress(addr);

            // TCPへ接続
            // TCPクライアントを起動
            if (tcpClient->Initialize(appPortIndex, hostAddress) == false)
                return false;

            // 入出許可を受信する
            //if (tcpClient->Recieve())
            //    ConversionCommandFromData(tcpClient->GetBuffer());
            std::cout << "自分の識別番号:" << ownListIndex << std::endl;
            for (int i = 0; i < MaxMemberSize; i++)
            {
                //if (joinPermitCom->preJoinData[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                //otherGuestData[i] = joinPermitCom->preJoinData[i];
                //std::cout << "つながっているPCのIPAddress(";
                //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
                //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
                //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
                //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
                //std::cout << "ポート番号:" << (int)connectMemberList[i].addrTCP.sin_port << std::endl;
                //if (connectMemberList[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                // プレイヤーの参加情報をゲームシーンで処理
                //if ((int)joinPermitCom->ownIndex != i)
                //    SceneManager::Instance().JoinPlayer(i, TypeConversionShort3::ChangeFloat3Decimal2(joinPermitCom->joinPosition[i]));
            }

            // UDPサーバーの起動
            if (udpClient->Initialize(appPortIndex + ownListIndex, hostAddress) == false)
                return false;
        }
        ConversionCommandFromData(multicastReciever->GetBuffer());
    }
    return false;
}

//メッセージを送信
void GuestCS::SendChatMessage(char chatMessage[128])
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //chatCom.actionFrame = SceneManager::Instance().GetActionFrame() + ragTime;// 1秒後(10フレーム後)に表示
    //snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);
  
    tcpClient->Send((char*)&chatCom, sizeof(ChatCommand));

    for (int i = 0; i < MaxMemberSize; i++)
    {
        if (connectMemberList[i].addrUDP.sin_addr.S_un.S_addr == 0)continue;
        udpClient->Send(connectMemberList[i].addrUDP, (char*)&chatCom, sizeof(ChatCommand));
    }
}
