#include "Stdafx.h"
#include "server.h"

CommunicateBaseServer::~CommunicateBaseServer()
{
    Finalize();
}

bool CommunicateBaseServer::Initialize(int port)
{
    //�R���\�[����ʂ�\��
    ConsoleFunc::OpenWindow();

    WSADATA data;
    //WSA�̏�����
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        //���������s
        Fail("WSA�̏������Ɏ��s���܂���");
        return false;
    }
    return true;
}

void CommunicateBaseServer::Finalize()
{
    //�\�P�b�g�̏I������
    int close = closesocket(ownSocket);
    if (close != 0)
    {
        Fail("�I�������Ɏ��s���܂��� �G���[�R�[�h�F");
    }

    //WSA�̉��
    WSACleanup();
    ConsoleFunc::WriteEndl("WSA���J�����܂���");
}

//������PC�̖��O��IP�A�h���X���擾
//true:�擾�ɐ�������
sockaddr_in CommunicateBaseServer::GetLocalAddress()
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

    std::string port = std::to_string(ownAddress.sin_port);

    char hostname[30];//host�̖��O���擾
    if (gethostname(hostname, 30) != 0)
    {
        Fail("�z�X�g�̖��O�̎擾�Ɏ��s���܂���");
        return { 0 };
    }

    if (getaddrinfo(hostname, port.c_str(), &tmpAddr, &addrInfo) != 0)
    {
        Fail("�z�X�g�̖��O����A�h���X�̎擾�Ɏ��s���܂���");
        return { 0 };
    }

    addr = *((sockaddr_in*)addrInfo->ai_addr);

    //servent* serv;
    //serv = getservbyname(hostname, 0);
    //if (serv != nullptr)
    //{
    //    Fail("�|�[�g�ԍ��̎擾�Ɏ��s���܂���");
    //}

    //std::cout << "���[�J��IPAddress(";
    //std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b1 << ".";
    //std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b2 << ".";
    //std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b3 << ".";
    //std::cout << (int)addr.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;;

    return addr;
}

