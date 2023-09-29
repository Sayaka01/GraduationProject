#pragma once

#include <WinSock2.h>//windows.h���O�ɋL�q����

#include <list>
#include <string>
#include <memory>

//�ʐM�V�X�e��
#include <d3d11.h>
#include "server_tcp.h"
#include "server_udp.h"
#include "server_multicast.h"
#include "client_tcp.h"
#include "client_udp.h"
#include "client_multicast.h"

//*********************************************************************************************
//  P2P�^�̃z�X�g�A�Q�X�g�̏���
//*********************************************************************************************

class PeerToPeerModel
{
public:   
    // �R���X�g���N�^
    PeerToPeerModel()
    {
        WSADATA data;
        //WSA�̏�����
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        {
            //���������s
            Fail("WSA�̏������Ɏ��s���܂���");
            return;
        }
        //�f�[�^��������
        for (int i = 0; i < MaxConnectNumber; i++)
        {
            ClearData(i);
        }

        sockaddr_in addr;

        //�N���C�A���g�̎󂯓�����̐ݒ�
        addrinfo* addrInfo = NULL;
        addrinfo tmpAddr;
        ZeroMemory(&tmpAddr, sizeof(addrinfo));
        //�ݒ肷�����ݒ�
        tmpAddr.ai_flags = 0;
        tmpAddr.ai_family = AF_INET;
        tmpAddr.ai_socktype = SOCK_DGRAM;
        tmpAddr.ai_protocol = 0;

        std::string port = std::to_string(61000);

        char hostname[30];//host�̖��O���擾
        if (gethostname(hostname, 30) != 0)
        {
            Fail("�z�X�g�̖��O�̎擾�Ɏ��s���܂���");
            return ;
        }

        if (getaddrinfo(hostname, port.c_str(), &tmpAddr, &addrInfo) != 0)
        {
            Fail("�z�X�g�̖��O����A�h���X�̎擾�Ɏ��s���܂���");
            return ;
        }

        addr = *((sockaddr_in*)addrInfo->ai_addr);
        strcat_s(inputHostAddress, std::to_string(addr.sin_addr.S_un.S_un_b.s_b1).c_str());
        strcat_s(inputHostAddress, ".");
        strcat_s(inputHostAddress, std::to_string(addr.sin_addr.S_un.S_un_b.s_b2).c_str());
        strcat_s(inputHostAddress, ".");
        strcat_s(inputHostAddress, std::to_string(addr.sin_addr.S_un.S_un_b.s_b3).c_str());
        strcat_s(inputHostAddress, ".");
        strcat_s(inputHostAddress, std::to_string(addr.sin_addr.S_un.S_un_b.s_b4).c_str());
    }
    // �f�X�g���N�^
    virtual ~PeerToPeerModel()
    {
        //�I������ۏ����l�֖߂�
        for (int i = 0; i < MaxConnectNumber; i++)
        {
            ClearData(i);
        }

        //for (auto& mess : recieveMessages)
        //{
        //    delete mess;
        //    mess = nullptr;
        //}
        //recieveMessages.clear();
        //for (auto& rMess : deleteRecieveMessages)
        //{
        //    delete rMess;
        //    rMess = nullptr;
        //}
        //deleteRecieveMessages.clear();

    }

    // �󂢂Ă���Index��Ԃ��A����I�[�o�[�̂Ƃ���-1��Ԃ�
    int GetEmptyIndex()
    {
        for (int i = 0; i < MaxConnectNumber; i++)
        {
            //�o�^����Ă��Ȃ��v�f��T���Ԃ�
            if (otherGuestData[i].socketTCP == INVALID_SOCKET)
                return i;
        }
        return -1;
    }

    // �����ݒ�
    virtual bool Initialize(ID3D11Device* device) { return false; }
    
    // ���[�U�[�̖��O��o�^,�����ݒ�
    virtual bool InitializePassName() { return false; }
   
    // �I���ݒ�
    virtual void Finalize() {}

    //ImGui���g�p�����`���b�g�V�X�e��
    void ChatMessageSystemGUI();

    // �`���b�g���b�Z�[�W���T�[�o�[�֑��M
    virtual void SendChatMessage(char chatMessage[128]) 
    {
        ConsoleFunc::WriteEndl("�Ή����Ă��Ȃ��֐����Ăяo���܂���(SendChatMessage)");
    }
   
    // �Q�X�g�̓����҂�
    virtual void WaitConnectFromGuest()
    {
        ConsoleFunc::WriteEndl("�Ή����Ă��Ȃ��֐����Ăяo���܂���(WaitConnectFromGuest)");
    }
    
    // �Ȃ��Ă����N���C�A���g�̏���ۑ�
    virtual void RegisterList(int index, ComputerData data) 
    {
        ConsoleFunc::WriteEndl("�Ή����Ă��Ȃ��֐����Ăяo���܂���(RegisterList)");
    }
    
    // �����̒l(IP�A�h���X�A�|�[�g�ԍ�)���炷�łɓo�^���Ă���N���C�A���g�����ׂ�
    // �߂�l�i�����̏ꍇ�F�o�^���Ă���index�@�V�K�̏ꍇ�F�[�P�j
    virtual int CheckAlreadyConnect(sockaddr_in* addr)
    {
        ConsoleFunc::WriteEndl("�Ή����Ă��Ȃ��֐����Ăяo���܂���(CheckAlreadyConnect)");
        return 10;
    }

    // �����Ă����R�}���h����������
    virtual void CommandFromData(char* buff) {}

    // �f�[�^���󂯎�������̏���
    virtual void Recieve() {}

    // �q�����Ă��鑊��S���Ƀf�[�^�𑗐M
    virtual void SendAll(char* data, int size) {}

    // TCP�ʐM���g�p���ē���̑���(Index�Ŏw��)�Ƀf�[�^�𑗐M
    virtual void SendTCP(char* data, int size,int index =0) {}
    // �}���`�L���X�g���g�p���ăf�[�^��S���ɑ��M
    virtual void SendByMulticast(char* data, int size) {}

    // �����ȂǂɎ��s�����Ƃ��̏���
    void Fail(std::string failMessage)
    {
        // �G���[���e��\������
        failMessage += " �G���[�R�[�h�F" + std::to_string(WSAGetLastError());
        ConsoleFunc::WriteEndl(failMessage.c_str());
    }

    // �R���\�[����ʂɏo��
    void WriteDataInConsoleAllTCP()
    {
        for (int i = 0; i < MaxConnectNumber; i++)
        {
            std::cout << "IPAddress���X�g" << i << ":(";
            std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
            std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
            std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
            std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
            std::cout << "�|�[�g�ԍ�:" << (int)otherGuestData[i].addrTCP.sin_port << std::endl;
        }

    }

    // �R���\�[����ʂɏo��
    void WriteDataInConsoleIndexTCP(int index)
    {
        std::cout << "IPAddress���X�g" << index << ":(";
        std::cout << (int)otherGuestData[index].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
        std::cout << (int)otherGuestData[index].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
        std::cout << (int)otherGuestData[index].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
        std::cout << (int)otherGuestData[index].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
        std::cout << "�|�[�g�ԍ�:" << (int)otherGuestData[index].addrTCP.sin_port << std::endl;
    }

    // �R���\�[����ʂɏo��
    void WriteDataInConsoleIndexUDP(int index)
    {
        std::cout << "IPAddress���X�g" << index << ":(";
        std::cout << (int)otherGuestData[index].addrUDP.sin_addr.S_un.S_un_b.s_b1 << ".";
        std::cout << (int)otherGuestData[index].addrUDP.sin_addr.S_un.S_un_b.s_b2 << ".";
        std::cout << (int)otherGuestData[index].addrUDP.sin_addr.S_un.S_un_b.s_b3 << ".";
        std::cout << (int)otherGuestData[index].addrUDP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
        std::cout << "�|�[�g�ԍ�:" << (int)otherGuestData[index].addrUDP.sin_port << std::endl;
    }

    // �f�[�^�̏�����
    void ClearData(int index)
    {
        otherGuestData[index].socketTCP = INVALID_SOCKET;
        otherGuestData[index].addrTCP.sin_addr.S_un.S_addr = 0;
        otherGuestData[index].addrTCP.sin_port = 0;
        otherGuestData[index].addrTCP.sin_family = 0;
        otherGuestData[index].addrUDP.sin_addr.S_un.S_addr = 0;
        otherGuestData[index].addrUDP.sin_port = 0;
        otherGuestData[index].addrUDP.sin_family = 0;
        otherGuestData[index].userName = "";
    }

    // �w���TCP�A�h���X����UDP�A�h���X���擾
    sockaddr_in ChangeUDPAddress(sockaddr_in addr);

    // �Z�b�^�[�ƃQ�b�^�[
    void SetUserName(std::string name) { playerName = name; }
    void SetPassword(std::string pass) { password = pass; }
    //void SetInitialPos(int i, DirectX::XMFLOAT3 pos) { initialPos[i] = pos; }
    std::string GetPlayerUserName() { return playerName; }
    int GetListIndex() { return listIndex; }
    char GetAction() { return action; }
    char* GetBuffer() { return buffer; }
    char* GetHostAddress() { return inputHostAddress; }
    std::string GetOtherName(int i) { return otherGuestData[i].userName; }

    char inputHostAddress[32]={};

protected:

    struct ChatData
    {
        ChatData(std::string mess, int fr) :message(mess), frame(fr) {}
        std::string message;
        int frame;
    };

    std::list<std::string> messageList;
    char mess[50];
    std::string playerName;
    std::string password;
    int ragTime = 10;
    //std::vector<ChatData*> recieveMessages;
    //std::vector<ChatData*> deleteRecieveMessages;
    //�ʐM���Ă���N���C�A���g�ɍő�l��
    static const int MaxConnectNumber = 4;
    //0�Ԗ�:�T�[�o�[���g�̏��
    ComputerData otherGuestData[MaxConnectNumber]{};//�N���C�A���g�̏����i�[
    char action;//��M�����f�[�^�̈�ԏ���(���̍s�������邩�����Ă��鐮�����i�[)
    char* buffer;
    int listIndex;//������Index�ԍ�
};

// �z�X�g���̏���
class HostP2P:public PeerToPeerModel
{
public:
    HostP2P() = default;
    ~HostP2P()override;

    bool Initialize(ID3D11Device* device) override;
    void Finalize() override;

    // �Q�X�g�̓����҂�
    void WaitConnectFromGuest() override;

    // ���[�U�[�̖��O��o�^,�����ݒ�
    bool InitializePassName() override;

    // �`���b�g���b�Z�[�W���T�[�o�[�֑��M
    void SendChatMessage(char chatMessage[128]) override;

    // �Ȃ��Ă����N���C�A���g�̏���ۑ�
    void RegisterList(int index, ComputerData data) override;

    // �����̒l(IP�A�h���X�A�|�[�g�ԍ�)���炷�łɓo�^���Ă���N���C�A���g�����ׂ�
    // �߂�l�i�����̏ꍇ�F�o�^���Ă���index�@�V�K�̏ꍇ�F�[�P�j
    int CheckAlreadyConnect(sockaddr_in* addr) override;

    void Recieve()override;

    void SendAll(char* data, int size)override;

    // �����Ă����R�}���h����������
    void CommandFromData(char* buff)override;

    void SendTCP(char* data, int size, int index = 0) override;

    void SendByMulticast(char* data, int size) override;
private:
    CommunicateTCPServer tcpServer;
    CommunicateUDPServer udpServer;
    CommunicateMultiCastServer multicastSender;
    int multiPortIndex = 62000;

    int portIndex = 64000;
};

// �Q�X�g���̏���
class GuestP2P:public PeerToPeerModel
{
public:
    GuestP2P() = default;
    ~GuestP2P() override;

    bool Initialize(ID3D11Device* device) override;
    void Finalize() override {};

    // �`���b�g���b�Z�[�W���T�[�o�[�֑��M
    void SendChatMessage(char chatMessage[128]) override;
   
    // ���[�U�[�̖��O��o�^,�����ݒ�
    bool InitializePassName() override;

    // �f�[�^����M
    void Recieve()override;

    // �ʐM���̑���S���Ɍ����ăf�[�^�𑗐M
    void SendAll(char* data, int size)override;

    // �����Ă����R�}���h����������
    void CommandFromData(char* buff)override;

    // TCP���g�p���ăf�[�^�𑗐M
    void SendTCP(char* data, int size, int index = 0) override;
private:
    // TCP�p�̃N���C�A���g
    CommunicateTCPClient tcpClient;
    // UDP�p�̃T�[�o�[
    CommunicateUDPServer udpServer;
    // �}���`�L���X�g����󂯎���
    CommunicateMultiCastClient multicastReciever;

    int portIndex = 64000;
    int multiPortIndex = 62000;
};