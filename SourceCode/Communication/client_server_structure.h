#pragma once

#include "../System/console.h"

#include "server_tcp.h"
#include "server_udp.h"
#include "server_multicast.h"

#include "client_tcp.h"
#include "client_udp.h"
#include "client_multicast.h"



//*********************************************************************************************
//  �N���T�o�^�̃z�X�g�A�Q�X�g�̏���
//*********************************************************************************************

class ClientServerModel
{
public:
    // �ʐM�ɕK�v�ȃf�[�^
    struct ConnectData
    {

    };

public:
    // �R���X�g���N�^
    ClientServerModel();
    // �f�X�g���N�^
    virtual ~ClientServerModel();


    // �����ȂǂɎ��s�����Ƃ��̏���
    void Fail(std::string failMessage)
    {
        // �G���[���e��\������
        failMessage += " �G���[�R�[�h�F" + std::to_string(WSAGetLastError());
        ConsoleFunc::WriteEndl(failMessage.c_str());
    }

    // �R���\�[����ʂɒʐM���̑���̃��X�g��S�ĕ\��
    void WriteConsoleConnectListAll();

    // �R���\�[����ʂɎw���IP�A�h���X��\��
    void WriteConsoleIPAdress(sockaddr_in srcAddr);

    // �f�[�^�̏�����
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

    // �󂢂Ă���Index��Ԃ��A����I�[�o�[�̂Ƃ���-1��Ԃ�
    int GetEmptyIndex()
    {
        for (int i = 0; i < MaxMemberSize; i++)
        {
            //�o�^����Ă��Ȃ��v�f��T���Ԃ�
            if (connectMemberList[i].socketTCP == INVALID_SOCKET)
                return i;
        }
        return -1;
    }

    //��������
    virtual bool Initialize();
    
    // �I������
    virtual void Finalize();

    // �f�[�^�𑗐M
    virtual void SendData() {}

    // �`���b�g���b�Z�[�W�𑗐M
    virtual void SendChatMessage(char chatMessage[128]) {}

    // �f�[�^����M
    virtual void RecieveData() {}

    // �Ȃ��Ă����N���C�A���g�̏���ۑ�
    virtual void RegisterList(int index, ComputerData data) {}

    // �w���TCP�A�h���X����UDP�A�h���X���擾
    sockaddr_in ChangeUDPAddress(sockaddr_in srcAddr);

    // �����Ă����R�}���h����������
    virtual void ConversionCommandFromData(char* buff) {}

    // �ڑ��҂�
    virtual bool WaitingConnect() { return false; }

    // �����̒l(IP�A�h���X�A�|�[�g�ԍ�)���炷�łɓo�^���Ă���N���C�A���g�����ׂ�
// �߂�l�i�����̏ꍇ�F�o�^���Ă���index�@�V�K�̏ꍇ�F�[�P�j
    virtual int CheckAlreadyConnect(sockaddr_in* addr)
    {
        ConsoleFunc::WriteEndl("�Ή����Ă��Ȃ��֐����Ăяo���܂���(CheckAlreadyConnect)");
        return 10;
    }

public:
    // �ڑ��o����ő�l��
    static const int MaxMemberSize = 4;
    // �ʐM����̃��X�g
    ComputerData connectMemberList[MaxMemberSize]{};

    int appPortIndex = 63000;
    int multiPortIndex = 62000;

    char action;//��M�����f�[�^�̈�ԏ���(���̍s�������邩�����Ă��鐮�����i�[)
    char* buffer;
    int ownListIndex{ 0 };

    // �}���`�L���X�g�T�[�o�[
    CommunicateMultiCastServer* multicastSender{ nullptr };

    // �}���`�L���X�g�N���C�A���g
    CommunicateMultiCastClient* multicastReciever{ nullptr };
};

class HostCS :public ClientServerModel
{
public:
    // �R���X�g���N�^
    HostCS();
    // �f�X�g���N�^
    ~HostCS();

    // ��������
    bool Initialize() override;

    // �I������
    void Finalize() override;

    // �f�[�^�𑗐M
    void SendData() override;

    // �f�[�^����M
    void RecieveData() override;

    // �Q�X�g�̐ڑ��҂�
    void Accept();

    // �Ȃ��Ă����N���C�A���g�̏���ۑ�
    void RegisterList(int index, ComputerData data) override;

    // �����̒l(IP�A�h���X�A�|�[�g�ԍ�)���炷�łɓo�^���Ă���N���C�A���g�����ׂ�
    // �߂�l�i�����̏ꍇ�F�o�^���Ă���index�@�V�K�̏ꍇ�F�[�P�j
    int CheckAlreadyConnect(sockaddr_in* addr) override;

    // �`���b�g���b�Z�[�W�𑗐M
    void SendChatMessage(char chatMessage[128]) override;

    // �����Ă����R�}���h����������
    void ConversionCommandFromData(char* buff) override;

    // �ڑ��҂�
    bool WaitingConnect() override;

public:
    // TCP�T�[�o�[
    CommunicateTCPServer* tcpServer{ nullptr };

    // UDP�T�[�o�[
    CommunicateUDPServer* udpServer{ nullptr };


};


class GuestCS :public ClientServerModel
{
public:
    // �R���X�g���N�^
    GuestCS();
    // �f�X�g���N�^
    ~GuestCS();

    // ��������
    bool Initialize() override;

    //�I������
    void Finalize() override;

    // �f�[�^�𑗐M
    void SendData() override;

    // �f�[�^����M
    void RecieveData() override;

    // �����Ă����R�}���h����������
    void ConversionCommandFromData(char* buff) override;

    // �ڑ��҂�
    bool WaitingConnect() override;

    void SendChatMessage(char chatMessage[128])override;

public:
    // TCP�N���C�A���g
    CommunicateTCPClient* tcpClient{ nullptr };

    // UDP�N���C�A���g
    CommunicateUDPClient* udpClient{ nullptr };

    char hostAddress[32] = {};
};

