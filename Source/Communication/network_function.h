#pragma once
// 通信、ネットワークに関する機能や設定

#define MAX_BUFFER_SIZE 512

//---- 通信用コマンド　なんの行動をするか ----
enum class ActionCommandType
{
    Join = 0,           // 入出
    JoinPermit,         // 入出許可
    JoinRejected,       // 入室拒否
    Exit,               // 退出
    ExitPermit,         // 退出許可
    ExitNotice,         // 退出通知
    IncreaseGuest,      // ゲスト増員
    Chat,               // メッセージの送受信
};

// 通信する為に必要なデータ(ソケット、アドレス)
struct ComputerData
{
    SOCKET socketTCP = INVALID_SOCKET;  // TCPのソケット
    sockaddr_in addrTCP;                // TCPのアドレス情報
    sockaddr_in addrUDP;                // UDPのアドレス情報
    std::string userName;               // 名前情報
};

// チャットメッセージ用コマンド
struct ChatCommand
{
    char actionType = (char)ActionCommandType::Chat;
    int actionFrame = 0;                                // 実行するアクションフレーム数
    char message[128];                                  // チャットのメッセージ
};

// 参加コマンド
struct JoinCommand
{
    char actionType = (char)ActionCommandType::Join;
    char roomIndex;                 //部屋番号
    char password;                  //パスワード
};

// 退出コマンド
struct ExitCommand
{
    char actionType = (char)ActionCommandType::Exit;
    char index = 0;
};

// 参加許可コマンド
struct JoinPermitCommand
{
    char actionType = (char)ActionCommandType::JoinPermit;
    char index = 0;
    ComputerData memberList[4];
};

// 
struct IncreaseGuestCommand
{
    char actionType = (char)ActionCommandType::IncreaseGuest;
    char index = 0;
    ComputerData newGuestData;
};

