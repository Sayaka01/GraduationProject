#pragma once
#include <WinSock2.h>//windows.h���O�ɋL�q����
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include "../System/console.h"
#include "network_function.h"

//----------------------------------------------------------------------------------------------
//      TCP,UDP�ʐM�̃N���C�A���g
//----------------------------------------------------------------------------------------------
class CommunicateBaseClient
{
public:
    ////�N���C�A���g�N���X���C���X�^���X��
    //static CommunicateClient& Instance()
    //{
    //    static CommunicateClient instance;
    //    return instance;
    //}

    


    CommunicateBaseClient() = default;
    virtual ~CommunicateBaseClient();

    //TCP�ʐM�̏����ݒ�
    virtual bool Initialize(int port, char* hostAddress);
    virtual bool Initialize(int port);

    //�\�P�b�g�̏I������
    virtual void Finalize();

    void ClearBuffer(){
        memset(buffer, '\0', sizeof(buffer));
    }

    //������PC�̖��O��IP�A�h���X���擾
    //true:�擾�ɐ�������
    sockaddr_in GetHostNameAndAddress(int port);

    //�����ȂǂɎ��s�����Ƃ��̏���
    // failMessage :�������s���ɏo���������b�Z�|�W
    void Fail(std::string failMessage)
    {
        //�G���[���e��\������
        failMessage += " �G���[�R�[�h�F"+std::to_string(WSAGetLastError());
        ConsoleFunc::WriteEndl(failMessage.c_str(),ConsoleFunc::Red);

        completeInitialize = false;
    }

    //���������ꍇ��1�A���s�����炻��ȊO(0)
    bool ChangeStringToLong(ADDRESS_FAMILY family, std::string ipStr, ULONG& purposeIp/*�ϊ���̃A�h���X*/)
    {
        ULONG ip_num = 0;// �ϊ���IP�A�h���X
        int pton_result = inet_pton(family, ipStr.c_str(), &ip_num);
        // IP�A�h���X�ϊ�������
        if (pton_result == 1)
        {
            purposeIp = ip_num;
            return true;
        }
        else
        {
            //Fail("IP�̎擾�Ɏ��s���܂���");
            return false;
        }

    }

    //�ϐ��̃Q�b�^�[
    //char GetAction() { return action; }
    char* GetBuffer() { return buffer; }
protected:   
    sockaddr_in ownAddress;
    bool completeInitialize{ false };//���������S�Đ���Ɏ��s���ꂽ��

    //char action{ (char)-1 };//��M�����f�[�^�̈�ԏ���(���̍s�������邩�����Ă��鐮�����i�[)
    char buffer[MAX_BUFFER_SIZE]{};//�T�[�o�[�����M�����f�[�^���i�[

    //ComputerData joinedObjectData[4];//�q�����Ă���R���s���[�^�[�̃f�[�^
public:
    char inputHostAddress[32]{};

};