#pragma once
// �ʐM�A�l�b�g���[�N�Ɋւ���@�\��ݒ�

#define MAX_BUFFER_SIZE 512

//---- �ʐM�p�R�}���h�@�Ȃ�̍s�������邩 ----
enum class ActionCommandType
{
    Join = 0,           // ���o
    JoinPermit,         // ���o����
    JoinRejected,       // ��������
    Exit,               // �ޏo
    ExitPermit,         // �ޏo����
    ExitNotice,         // �ޏo�ʒm
    IncreaseGuest,      // �Q�X�g����
    Chat,               // ���b�Z�[�W�̑���M
};

// �ʐM����ׂɕK�v�ȃf�[�^(�\�P�b�g�A�A�h���X)
struct ComputerData
{
    SOCKET socketTCP = INVALID_SOCKET;  // TCP�̃\�P�b�g
    sockaddr_in addrTCP;                // TCP�̃A�h���X���
    sockaddr_in addrUDP;                // UDP�̃A�h���X���
    std::string userName;               // ���O���
};

// �`���b�g���b�Z�[�W�p�R�}���h
struct ChatCommand
{
    char actionType = (char)ActionCommandType::Chat;
    int actionFrame = 0;                                // ���s����A�N�V�����t���[����
    char message[128];                                  // �`���b�g�̃��b�Z�[�W
};

// �Q���R�}���h
struct JoinCommand
{
    char actionType = (char)ActionCommandType::Join;
    char roomIndex;                 //�����ԍ�
    char password;                  //�p�X���[�h
};

// �ޏo�R�}���h
struct ExitCommand
{
    char actionType = (char)ActionCommandType::Exit;
    char index = 0;
};

// �Q�����R�}���h
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

