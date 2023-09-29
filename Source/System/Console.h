#pragma once
#include <wtypes.h>
#include <winnt.h>

//#define OpenConsoleWind

//******************************************************
// �R���\�[����ʂɑ΂��鏈��
//******************************************************
class ConsoleFunc
{
public:
    // �����̐F
    enum FontColor
    {
        White   = 7,    // ��
        Blue    = 9,    // ��
        Green   = 10,   // ��
        SkyBlue = 11,   // ��
        Red     = 12,   // ��
    };

    // �R���\�[����ʂ��J��
    static void OpenWindow();
    // �R���\�[����ʂ����
    static void CloseWiindow();

    // �R���\�[����ʂɕ������o�͂���
    static void Write(const char* _message, FontColor _color = White);
    // �R���\�[����ʂɕ������o�͂���i�����ɉ��s�A���j
    static void WriteEndl(const char* _message, FontColor _color = White);

    // FLOAT3�Ȃǂ̃f�[�^���R���\�[����ʂɏo��
    //static void WriteData(const char* _message, const DirectX::XMFLOAT3& _val, FontColor _color = White);

    // �R���\�[����ʂ̏����T�C�Y
    static constexpr SMALL_RECT WindowSize = { 0, 0, 50, 25 };

private:

    inline static HANDLE Handle{};

};