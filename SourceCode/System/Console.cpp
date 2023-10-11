#include "Stdafx.h"
#include "console.h"
#include<Windows.h>
#include<cstdio>
#include<iostream>

// コンソール画面を開く
void ConsoleFunc::OpenWindow()
{

//#ifdef OpenConsoleWind 
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    setvbuf(stdout, nullptr, _IONBF, 0);
    Handle = GetStdHandle(STD_OUTPUT_HANDLE);	// DWORD nStdHandle
    if (Handle != INVALID_HANDLE_VALUE)
    {
        SetConsoleWindowInfo(
            Handle, // HANDLE hConsoleOutput
            TRUE,           // BOOL bAbsolute
            &ConsoleFunc::WindowSize);// CONST SMALL_RECT *lpConsoleWindow
    }
    // コンソールから閉じたときに異常終了するのを回避する用
    SetConsoleCtrlHandler([](DWORD)->BOOL {return true; }, TRUE);
//#endif   

}

// コンソール画面を閉じる
void ConsoleFunc::CloseWiindow()
{
//#ifdef OpenConsoleWind 
    FreeConsole();
//#endif
}

// コンソール画面に文字を出力する
void ConsoleFunc::Write(const char* _message, FontColor _color)
{
    SetConsoleTextAttribute(Handle, _color);
    std::cout << _message;
    SetConsoleTextAttribute(Handle, FontColor::White);
}

// コンソール画面に文字を出力する（文末に改行アリ）
void ConsoleFunc::WriteEndl(const char* _message, FontColor _color)
{
    SetConsoleTextAttribute(Handle, _color);
    std::cout << _message << std::endl;
    SetConsoleTextAttribute(Handle, FontColor::White);
}
