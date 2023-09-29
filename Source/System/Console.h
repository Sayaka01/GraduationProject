#pragma once
#include <wtypes.h>
#include <winnt.h>

//#define OpenConsoleWind

//******************************************************
// コンソール画面に対する処理
//******************************************************
class ConsoleFunc
{
public:
    // 文字の色
    enum FontColor
    {
        White   = 7,    // 白
        Blue    = 9,    // 青
        Green   = 10,   // 緑
        SkyBlue = 11,   // 水
        Red     = 12,   // 赤
    };

    // コンソール画面を開く
    static void OpenWindow();
    // コンソール画面を閉じる
    static void CloseWiindow();

    // コンソール画面に文字を出力する
    static void Write(const char* _message, FontColor _color = White);
    // コンソール画面に文字を出力する（文末に改行アリ）
    static void WriteEndl(const char* _message, FontColor _color = White);

    // FLOAT3などのデータをコンソール画面に出力
    //static void WriteData(const char* _message, const DirectX::XMFLOAT3& _val, FontColor _color = White);

    // コンソール画面の初期サイズ
    static constexpr SMALL_RECT WindowSize = { 0, 0, 50, 25 };

private:

    inline static HANDLE Handle{};

};