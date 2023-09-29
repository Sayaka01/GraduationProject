#include "Stdafx.h"
#include "client.h"
#include <process.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#include "server.h"

CommunicateBaseClient::~CommunicateBaseClient()
{
    Finalize();
}

bool CommunicateBaseClient::Initialize(int port, char* hostAddress)
{
    //�R���\�[����ʂ�\��
    ConsoleFunc::OpenWindow();

    //FD_ZERO(&clientList);

    WSADATA data{};
    int value = WSAStartup(MAKEWORD(2, 2), &data);//0���Ԃ��Ă���Ɩ��Ȃ�����
    if (value != 0)
    {
        //���s
        Fail("�������Ɏ��s���܂���");
        return false;
    }
    return true;
}

bool CommunicateBaseClient::Initialize(int port)
{
    //�R���\�[����ʂ�\��
    ConsoleFunc::OpenWindow();

    //FD_ZERO(&clientList);

    WSADATA data{};
    int value = WSAStartup(MAKEWORD(2, 2), &data);//0���Ԃ��Ă���Ɩ��Ȃ�����
    if (value != 0)
    {
        //���s
        Fail("�������Ɏ��s���܂���");
        return false;
    }
    return true;
}

void CommunicateBaseClient::Finalize()
{
    ConsoleFunc::WriteEndl("�ڑ����I�����܂���");
    //WSA�̊J��
    WSACleanup();

    ConsoleFunc::WriteEndl("WSA���J�����܂���");

}


//������PC�̖��O��IP�A�h���X���擾
//true:�擾�ɐ�������
sockaddr_in CommunicateBaseClient::GetHostNameAndAddress(int port)
{
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

    std::string p = std::to_string(port);

    char hostname[30];//host�̖��O���擾
    if (gethostname(hostname, 30) != 0)
    {
        Fail("�z�X�g�̖��O�̎擾�Ɏ��s���܂���");
        return { 0 };
    }

    if (getaddrinfo(hostname, p.c_str(), &tmpAddr, &addrInfo) != 0)
    {
        Fail("�z�X�g�̖��O����A�h���X�̎擾�Ɏ��s���܂���");
        return { 0 };
    }

    addr = *((sockaddr_in*)addrInfo->ai_addr);

    servent* serv;
    serv = getservbyname(hostname, 0);
    if (serv != nullptr)
    {
        Fail("�|�[�g�ԍ��̎擾�Ɏ��s���܂���");
    }
    //else
    //{
        //_Port = (int)serv->s_port;
    //}

    std::cout << "�z�X�g��IPAddress(";
    std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b1 << ".";
    std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b2 << ".";
    std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b3 << ".";
    std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;;

    return addr;
}
