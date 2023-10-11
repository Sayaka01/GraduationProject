#include "Stdafx.h"
#include "client_multicast.h"

// �f�X�g���N�^
CommunicateMultiCastClient::~CommunicateMultiCastClient()
{
}

// �}���`�L���X�g�ʐM�̏����ݒ�
bool CommunicateMultiCastClient::Initialize(int port)
{
    //�����ݒ�
    CommunicateBaseClient::Initialize(port);

    //�\�P�b�g�̐���
    ownSocket = socket(AF_INET, SOCK_DGRAM, 0);//UDP
    if (ownSocket == INVALID_SOCKET)
    {
        //���s
        Fail("�\�P�b�g�̐����Ɏ��s���܂���");
        return false;
    }


    //�m���u���b�L���O�ɐݒ�
    u_long val = 1;
    ioctlsocket(ownSocket, FIONBIO, &val);
    //**************************************************************
    // �}���`�L���X�g�̐ݒ�
    //**************************************************************
    ownAddress.sin_family = AF_INET;
    ownAddress.sin_port = htons(port);
    ownAddress.sin_addr.S_un.S_addr = INADDR_ANY;
//    Address = GetHostNameAndAddress(port);
    localAddress = GetLocalAddress();

    int one = 1;
    setsockopt(ownSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(one));

    // �\�P�b�g�ɃA�h���X�����o�C���h
    int bind_result = bind(ownSocket, (struct sockaddr*)&ownAddress, sizeof(ownAddress));
    // �\�P�b�g�o�C���h���s���̃G���[����
    if (bind_result != 0)
    {
        Fail("�}���`�L���X�g�̃\�P�b�g�Ƃ̊֘A�t���Ɏ��s���܂���");
        return false;
    }

    // �}���`�L���X�g���N�G�X�g�̃Z�b�g
    memset(&MulticastRequest, 0, sizeof(MulticastRequest));
    //�f�t�H���g�̃}���`�L���X�g�C���^�[�t�F�[�X��I��
    MulticastRequest.imr_interface.S_un.S_addr = INADDR_ANY;
    if (!ChangeStringToLong(ownAddress.sin_family, multicast_ip, MulticastRequest.imr_multiaddr.S_un.S_addr))
    {
        Fail("IP�A�h���X�̕ϊ��Ɏ��s");
        return false;
    }
    // UDP�}���`�L���X�g�\�P�b�g�I�v�V�����Z�b�g
    int set_opt_result = setsockopt(ownSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&MulticastRequest, sizeof(MulticastRequest));
   
    // �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[����
    if (set_opt_result != 0)
    {        
        Fail("�}���`�L���X�g�\�P�b�g�I�v�V�����Z�b�g�Ɏ��s���܂���");
        return false;
    }

    ConsoleFunc::WriteEndl("�}���`�L���X�g�N���C�A���g�̐ݒ�ɐ������܂����B", ConsoleFunc::Blue);
    completeInitialize = true;
    return true;
}

// �I������
void CommunicateMultiCastClient::Finalize()
{
    CommunicateBaseClient::Finalize();
}

// �f�[�^�̎�M����
bool CommunicateMultiCastClient::Recieve()
{
    //if (!completeInitialize)return false;

    int size = sizeof(sockaddr_in);

    int recvDataSize = recvfrom(ownSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&fromAddr, &size);
    //�f�[�^�T�C�Y�����݂�IP�A�h���X����v���Ȃ����肩��f�[�^����M�����Ƃ��̂ݏ���
    if (recvDataSize > 0 /*&& fromAddr.sin_addr.S_un.S_addr != localAddress.sin_addr.S_un.S_addr*/)
    {
        ConsoleFunc::Write("multicast:"); 
        std::cout << (int)buffer[0] << std::endl;
        return true;
    }
    return false;
}

//������PC�̖��O��IP�A�h���X���擾
//true:�擾�ɐ�������
sockaddr_in CommunicateMultiCastClient::GetLocalAddress()
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
