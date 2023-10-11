#pragma once
#include <WinSock2.h>//windows.h���O�ɋL�q����

#include <string>
#include <iostream>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include "../System/console.h"
#include "network_function.h"

//----------------------------------------------------------------------------------------------
//      TCP,UDP�ʐM�̃T�[�o�[
//----------------------------------------------------------------------------------------------

class CommunicateBaseServer
{
public:
    CommunicateBaseServer() = default;
    virtual ~CommunicateBaseServer();

    //�ʐM�̏����ݒ�
    virtual bool Initialize(int port);
    //�\�P�b�g�̏I������
    virtual void Finalize();

    //������PC�̖��O��IP�A�h���X���擾
    //true:�擾�ɐ�������
    sockaddr_in GetLocalAddress();

    //�����ȂǂɎ��s�����Ƃ��̏���
    void Fail(std::string failMessage)
    {
        //�G���[���e��\������
        failMessage += " �G���[�R�[�h�F" + std::to_string(WSAGetLastError());
        ConsoleFunc::WriteEndl(failMessage.c_str());

        completeInitialize = false;
    }

    char GetAction() { return action; }
    char* GetBuffer() { return buffer; }

    SOCKET& GetSocket() { return ownSocket; }

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

protected:
    char buffer[MAX_BUFFER_SIZE];//��M�����f�[�^���i�[
    char action;//��M�����f�[�^�̈�ԏ���(���̍s�������邩�����Ă��鐮�����i�[)

    struct sockaddr_in ownAddress;
    bool completeInitialize = false;//���������S�Đ���Ɏ��s���ꂽ��

    SOCKET ownSocket;//�����̃\�P�b�g

};

