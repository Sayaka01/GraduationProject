#include "Stdafx.h"
#include "server_udp.h"

CommunicateUDPServer::~CommunicateUDPServer()
{
    Finalize();
}

//�ʐM�̏����ݒ�
bool CommunicateUDPServer::Initialize(int port)
{
    //UDP�T�[�o�[�Ŏ󂯎�����A�h���X����N���C�A���g�̃A�h���X��ۑ��������Ɍ������đ���

    CommunicateBaseServer::Initialize(port);

    //�N���C�A���g�󂯓�����̐ݒ�
    ownAddress.sin_family = AF_INET;
    ownAddress.sin_port = htons(port);
    ownAddress.sin_addr.S_un.S_addr = INADDR_ANY;

    addrFromName = GetLocalAddress();

    ownSocket = socket(AF_INET, SOCK_DGRAM, 0);

    //�\�P�b�g�̐���
    //TCP:SOCK_STREAM
    //UDP:SOCK_DGRAM
    //serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ownSocket == INVALID_SOCKET)
    {
        Fail("�\�P�b�g�̐����Ɏ��s���܂���");
        return false;
    }

    //�\�P�b�g�Ǝ󂯓�������֘A�t����
    if (bind(ownSocket, (struct sockaddr*)&ownAddress, sizeof(ownAddress)) != 0)
    {
        Fail("�\�P�b�g�Ǝ󂯓�����̊֘A�t���Ɏ��s���܂���");
        return false;
    }
    //�m���u���b�L���O�ɐݒ�
    u_long nonBlock = 1;
    ioctlsocket(ownSocket, FIONBIO, &nonBlock);

    ConsoleFunc::WriteEndl("�T�[�o�[������ɋN�����܂����iUDP�j", ConsoleFunc::Blue);
    completeInitialize = true;

    return true;
}

void CommunicateUDPServer::Finalize()
{
}

//�N���C�A���g��UDP�ʐM���g�p���ăf�[�^�𑗐M
void CommunicateUDPServer::Send(sockaddr_in receiverAddr, char* data, int size)
{
    sendto(ownSocket, data, size, 0, (struct sockaddr*)&receiverAddr, sizeof(sockaddr_in));
}

//�N���C�A���g����UDP�ʐM���g�p���ăf�[�^����M
sockaddr_in CommunicateUDPServer::Recieve()
{
    //if (!completeInitialize)return { 0 };

    int recieveCount{ 0 };
    int size = sizeof(sockaddr_in);
    sockaddr_in fromAddr;//�ꎞ�I�ɕۑ����Ă����n�R

    recieveCount = recvfrom(ownSocket, (char*)&buffer, sizeof(buffer), 0, (sockaddr*)&fromAddr, &size);
    if (recieveCount > 0)
    {
        ConsoleFunc::Write("UDPs:");
        //if (buffer[0] == (char)ActionCommandType::Join)
        //{
        //    //���o����
        //    JoinRegisterUDP(fromAddr);
        //    completeInitialize = true;
        //}
        //else
        //{
        

            //���̑��̍X�V����
        //CommandFromData();
        //}
        return fromAddr;
    }
    return { 0 };
}