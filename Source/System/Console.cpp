#include "Stdafx.h"
#include "console.h"
#include<Windows.h>
#include<cstdio>
#include<iostream>

// �R���\�[����ʂ��J��
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
    // �R���\�[����������Ƃ��Ɉُ�I������̂��������p
    SetConsoleCtrlHandler([](DWORD)->BOOL {return true; }, TRUE);
//#endif   

}

// �R���\�[����ʂ����
void ConsoleFunc::CloseWiindow()
{
//#ifdef OpenConsoleWind 
    FreeConsole();
//#endif
}

// �R���\�[����ʂɕ������o�͂���
void ConsoleFunc::Write(const char* _message, FontColor _color)
{
    SetConsoleTextAttribute(Handle, _color);
    std::cout << _message;
    SetConsoleTextAttribute(Handle, FontColor::White);
}

// �R���\�[����ʂɕ������o�͂���i�����ɉ��s�A���j
void ConsoleFunc::WriteEndl(const char* _message, FontColor _color)
{
    SetConsoleTextAttribute(Handle, _color);
    std::cout << _message << std::endl;
    SetConsoleTextAttribute(Handle, FontColor::White);
}
