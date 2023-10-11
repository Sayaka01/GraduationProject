#include "Stdafx.h"
#include "p2p_structure.h"
//#include "../../imgui/imgui.h"
#include <ws2tcpip.h>
//#include <magic_enum.hpp>
//#include "hash.h"

HostP2P::~HostP2P()
{
}

// 初期設定
bool HostP2P::Initialize(ID3D11Device* device)
{
    PeerToPeerModel::Initialize(device);

    // TCPサーバーの起動
    if (tcpServer.Initialize(portIndex) == false)
        return false;

    // UDPサーバーの起動
    if (udpServer.Initialize(portIndex) == false)
        return false;

    // マルチキャスト用サーバー起動
    if (multicastSender.Initialize(multiPortIndex) == false)
        return false;

    // 情報を保存
    otherGuestData[0].socketTCP = tcpServer.GetSocket();
    otherGuestData[0].addrTCP = tcpServer.GetSocketAddress();
    otherGuestData[0].addrUDP = ChangeUDPAddress(tcpServer.GetSocketAddress());

    listIndex = 0;
    return true;
}

// ソケットの終了処理
void HostP2P::Finalize()
{
    for (int i = 0; i < MaxConnectNumber; i++)
    {
        int close = closesocket(otherGuestData[i].socketTCP);
        if (close != 0)
        {
            Fail("終了処理に失敗しました エラーコード：");
        }
    }
}

// ゲストの入室待ち
void HostP2P::WaitConnectFromGuest()
{
    // ゲストによるTCPサーバーへの通信待ち
    int emptyIndex = GetEmptyIndex();
    ComputerData data{};

    if (tcpServer.AcceptOnSelect(emptyIndex, &data)==true)//入室があった場合
    {
        // ゲストのデータを保管
        RegisterList(emptyIndex, data);
        tcpServer.SetList(&otherGuestData[emptyIndex].socketTCP);

        // 確認のためのメッセージを送信
        ChatCommand chatCom;
        chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
        char chatMessage[128] = { "thank you connecting." };
        strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);
        udpServer.Send(otherGuestData[emptyIndex].addrUDP, (char*)&chatCom, sizeof(ChatCommand));
      
    }
}

// ユーザーの名前を登録,初期設定
bool HostP2P::InitializePassName()
{
    char name[20]{};
    strcpy_s(name, strlen(playerName.c_str()) + 1, playerName.c_str());
    otherGuestData[listIndex].userName = std::string(name);
    return true;
}

// チャットメッセージを送信
void HostP2P::SendChatMessage(char chatMessage[128])
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //chatCom.actionFrame = SceneManager::Instance().GetActionFrame() + ragTime;// 1秒後(10フレーム後)に表示
    //snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);

    for (int i = 1; i < MaxConnectNumber; i++)
    {
        if (otherGuestData[i].addrUDP.sin_addr.S_un.S_addr != 0)
            udpServer.Send(otherGuestData[i].addrUDP,(char*)&chatCom, sizeof(ChatCommand));
    }
}

// つなげてきたクライアントの情報を保存
void HostP2P::RegisterList(int index, ComputerData data)
{
    //既に通信済かどうか確認する
    int connectIndex = CheckAlreadyConnect(&data.addrTCP);

    if (connectIndex >= 0)// 既に通信済の相手であれば
    {
        ConsoleFunc::WriteEndl("既に入出済です");
        return;
    }

    //新規の相手の場合
    otherGuestData[index] = data;

    ConsoleFunc::Write("新規クライアントを登録しました");
    std::cout << (int)index << std::endl;

    //参加許可コマンドを送信
    JoinPermitCommand joinPermitCom;
    joinPermitCom.actionType = (char)ActionCommandType::JoinPermit;
    joinPermitCom.index = index;
    tcpServer.Send(otherGuestData[index].socketTCP, (char*)&joinPermitCom, sizeof(JoinPermitCommand));

    // TCPのアドレスからUDPのアドレスへ変換
    otherGuestData[index].addrUDP = ChangeUDPAddress(otherGuestData[index].addrTCP);
    
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
int HostP2P::CheckAlreadyConnect(sockaddr_in* addr)
{
    for (int i = 0; i < MaxConnectNumber; i++)
    {
        // アドレスとポート番号が一致するか調べる
        if (addr->sin_addr.S_un.S_addr == otherGuestData[i].addrUDP.sin_addr.S_un.S_addr &&
            addr->sin_port == otherGuestData[i].addrUDP.sin_port)
            return i;
    }
    return -1;
}

// データを受け取った時の処理
void HostP2P::Recieve()
{
    buffer = nullptr;
    action = -1;
    sockaddr_in udpAddr;
    //if (udpServer.Recieve())
    // UDP通信で受信
    udpAddr = udpServer.Recieve();
    if (udpAddr.sin_addr.S_un.S_addr != 0)// 受信に成功した場合
    {
        // 送られてきたコマンドを処理
        CommandFromData(udpServer.GetBuffer());

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
    for (int i = 0; i < MaxConnectNumber; i++)
    {
        if (otherGuestData[i].socketTCP == INVALID_SOCKET)continue;
        if (tcpServer.RecieveOnSelect(otherGuestData[i].socketTCP))
            CommandFromData(tcpServer.GetBuffer());
    }
}

// 繋がっている相手全員にデータを送信
void HostP2P::SendAll(char* data,int size)
{
    for (int i = 1; i < MaxConnectNumber; i++)
    {
        if (otherGuestData[i].addrUDP.sin_addr.S_un.S_addr == 0)continue;
        udpServer.Send(otherGuestData[i].addrUDP,data, size);
    }
}

// 送られてきたコマンドを処理する
void HostP2P::CommandFromData(char* buff)
{
    buffer = buff;
    action = buffer[0];
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
        //Console::WriteEndl("チャット受信");
        //{
        //    ChatCommand* chatCom = (ChatCommand*)(buff);
        //    Console::Write(chatCom->message);
        //    Console::WriteEndl(std::to_string(chatCom->actionFrame).c_str());
        //    ChatData* chatData = new ChatData(chatCom->message, chatCom->actionFrame);
        //    recieveMessages.emplace_back(chatData);
        //}
        break;
    default:
        ConsoleFunc::Write("受信したコマンド：");
        //ConsoleFunc::WriteEndl(magic_enum::enum_name(static_cast<ActionCommandType>(action)).data());
        break;
    }
    std::cout << std::endl;
}

// TCP通信を使用して特定の相手(Indexで指定)にデータを送信
void HostP2P::SendTCP(char* data, int size, int index)
{
    // ソケットが登録されていればコマンドを送る
    if (otherGuestData[index].socketTCP == INVALID_SOCKET)return;
    tcpServer.Send(otherGuestData[index].socketTCP, data, size);
}

// マルチキャストを使用してデータを全員に送信
void HostP2P::SendByMulticast(char* data, int size)
{
    multicastSender.Send(data, size);
}

GuestP2P::~GuestP2P()
{
}

// 初期設定
bool GuestP2P::Initialize(ID3D11Device* device)
{
    PeerToPeerModel::Initialize(device);

    // TCPクライアントを起動
    if (tcpClient.Initialize(portIndex, inputHostAddress) == false)
        return false;
 
    // 入出許可を受信する
    if (tcpClient.Recieve())
        CommandFromData(tcpClient.GetBuffer());

    // UDPサーバーの起動
    if (udpServer.Initialize(portIndex + listIndex) == false)
        return false;

    // マルチキャスト用クライアント起動
    if (!multicastReciever.Initialize(multiPortIndex/*+listIndex*/))
        return false;

    // 名前の情報を受け取る
    if (tcpClient.Recieve())
        CommandFromData(tcpClient.GetBuffer());

    return true;
}

// チャットメッセージをサーバーへ送信
void GuestP2P::SendChatMessage(char chatMessage[128])
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //chatCom.actionFrame = SceneManager::Instance().GetActionFrame() + ragTime;// 1秒後(10フレーム後)に表示
//snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);

    for (int i = 0; i < MaxConnectNumber; i++)
    {
        if (otherGuestData[i].addrUDP.sin_addr.S_un.S_addr != 0)
            udpServer.Send(otherGuestData[i].addrUDP,(char*)&chatCom, sizeof(ChatCommand));
    }
}

// ユーザーの名前を登録,初期設定
bool GuestP2P::InitializePassName()
{
    char name[20]{};
    strcpy_s(name, strlen(playerName.c_str()) + 1, playerName.c_str());
    otherGuestData[listIndex].userName = std::string(name);

    //Hash(password);
    // ユーザー（自分）の情報をホストに送信
    //OwnDataCommand ownDataCom;
    //ownDataCom.actionType = static_cast<char>(ActionCommandType::GiveOwnData);
    //ownDataCom.guestIndex = listIndex;
    //ownDataCom.name = otherGuestData[listIndex].userName;
    //ownDataCom.password = password;
    //udpServer.Send(otherGuestData[0].addrUDP, (char*)&ownDataCom, sizeof(OwnDataCommand));
   
    // UDPを使用して送信されたデータ受信
    sockaddr_in recieveAddr{};
    recieveAddr = udpServer.Recieve();
    if (recieveAddr.sin_addr.S_un.S_addr != 0)
        CommandFromData(udpServer.GetBuffer());//受信したデータの処理

    // ホストからTCPを使用して送信されたデータを受信
    if (tcpClient.Recieve())
    {
        // 受信したデータの処理
        CommandFromData(tcpClient.GetBuffer());
        // 退出コマンドを受け取ったら
        if (action == static_cast<char>(ActionCommandType::Exit))
        {
            return false;
        }
    }
    return true;
}

// データを受け取った時の処理
void GuestP2P::Recieve()
{
    buffer = nullptr;
    action = -1;
    if (udpServer.Recieve().sin_addr.S_un.S_addr != 0)
        CommandFromData(udpServer.GetBuffer());
    //for (int i = 0; i < MaxConnectNumber; i++)
    if (tcpClient.RecieveOnSelect())
        CommandFromData(tcpClient.GetBuffer());
    if(multicastReciever.Recieve())
        CommandFromData(multicastReciever.GetBuffer());
}

// 繋がっている相手全員にデータを送信
void GuestP2P::SendAll(char* data, int size)
{
    for (int i = 0; i < MaxConnectNumber; i++)
    {
        // データがなければ送らない
        if (otherGuestData[i].addrUDP.sin_addr.S_un.S_addr == 0)continue;
        // 自分自身には送らない
        if (i == listIndex)continue;

        udpServer.Send(otherGuestData[i].addrUDP,data, size);
    }
}

// 送られてきたコマンドを処理する
void GuestP2P::CommandFromData(char* buff)
{
    buffer = buff;
    action = buff[0];

    switch (action)
    {
    case static_cast<int>(ActionCommandType::JoinPermit):// 入室許可
        ConsoleFunc::WriteEndl("入出許可を受信");
        {
            //JoinPermitCommand* joinPermitCom = (JoinPermitCommand*)(buff);
            //listIndex = (int)joinPermitCom->ownIndex;
            //SceneManager::Instance().SetHostStartActionFrame(joinPermitCom->actionFrame);
            std::cout << "自分の識別番号:" << listIndex << std::endl;
            for (int i = 0; i < MaxConnectNumber; i++)
            {
                //if (joinPermitCom->preJoinData[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                //otherGuestData[i] = joinPermitCom->preJoinData[i];
                std::cout << "つながっているPCのIPAddress(";
                std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
                std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
                std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
                std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
                std::cout << "ポート番号:" << (int)otherGuestData[i].addrTCP.sin_port << std::endl;
                if (otherGuestData[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
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
        //Console::WriteEndl("チャット受信");
        //{
        //    ChatCommand* chatCom = (ChatCommand*)(buff);
        //    Console::WriteEndl(chatCom->message);
        //    Console::WriteEndl(std::to_string(chatCom->actionFrame).c_str());
        //    ChatData* chatData = new ChatData(chatCom->message, chatCom->actionFrame);
        //    recieveMessages.emplace_back(chatData);
        //}
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

//TCPを使用して指定の相手へデータを送信
void GuestP2P::SendTCP(char* data, int size, int index)
{
    tcpClient.Send(data, size);
}

// 指定のTCPアドレスからUDPアドレスを取得
sockaddr_in PeerToPeerModel::ChangeUDPAddress(sockaddr_in srcAddr)
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

    std::cout << "ホストのIPAddress(";
    std::cout << (int)resultAddr.sin_addr.S_un.S_un_b.s_b1 << ".";
    std::cout << (int)resultAddr.sin_addr.S_un.S_un_b.s_b2 << ".";
    std::cout << (int)resultAddr.sin_addr.S_un.S_un_b.s_b3 << ".";
    std::cout << (int)resultAddr.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
    return resultAddr;
}