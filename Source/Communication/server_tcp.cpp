#include "Stdafx.h"
#include "server_tcp.h"

CommunicateTCPServer::~CommunicateTCPServer()
{
    Finalize();
}

// �ʐM�̏����ݒ�
bool CommunicateTCPServer::Initialize(int port)
{
    CommunicateBaseServer::Initialize(port);

    // �N���C�A���g�̎󂯓�����̐ݒ�
    ownAddress.sin_family = AF_INET;
    ownAddress.sin_port = htons(port);
    ownAddress.sin_addr.S_un.S_addr = INADDR_ANY;

    addrFromName = GetLocalAddress();

    // �\�P�b�g�̐���
    // TCP:SOCK_STREAM
    // UDP:SOCK_DGRAM
    ownSocket = socket(AF_INET, SOCK_STREAM, 0);
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

    //�T�[�o�[�̐ڑ��҂�
    listen(ownSocket, 10);

    ConsoleFunc::WriteEndl("�T�[�o�[������ɋN�����܂����iTCP�j", ConsoleFunc::Blue);

    //���X�g�̏�����
    FD_ZERO(&serverList);
    FD_ZERO(&clientList);

    FD_SET(ownSocket, &serverList);

    //clients.data[0].socketTCP = serverSockTCP;
    //clients.data[0].addrTCP = ownAddr;
    //std::cout << "�|�[�g�ԍ��iTCP�j" << clients.data[0].addrTCP.sin_port << std::endl;

    return true;
}

void CommunicateTCPServer::Finalize()
{
}

//select�֐����g�p���ă\�P�b�g�ɕω������������̂݃N���C�A���g�̎󂯓��ꏈ��
bool CommunicateTCPServer::AcceptOnSelect(int emptyIndex, ComputerData* newData)
{
    //if (!completeInitialize)return false;

    int len = sizeof(struct sockaddr_in);

    struct timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = 0;
    fd_set socketTmp;
    memcpy(&socketTmp, &serverList, sizeof(serverList));

    //�ω����������\�P�b�g�̐����������Ă���
    int variableSocketCount = 0;
    variableSocketCount = select(0, &socketTmp, NULL, NULL, &tm);
    if (variableSocketCount > 0)
    {
        newData->socketTCP = accept(ownSocket, (struct sockaddr*)&newData->addrTCP, &len);
        //�Q�X�g�̃\�P�b�g�̎擾�Ɏ��s�����Ƃ�
        if (newData->socketTCP == INVALID_SOCKET)
        {
            int code = WSAGetLastError();
            if (code != WSAEWOULDBLOCK)
            {
                //WSAEWOULDBLOCK=
                // �Ώۂ̃\�P�b�g���m���u���b�L���O�ɐݒ肳��Ă��邽�߁A�������s���܂���ł����B
                // �������s�������̂ł���΃u���b�L���O�ɐݒ肵�Ă��������B

                Fail("�\�P�b�g���m���u���b�L���O�ɐݒ肳��Ă��܂��B");
            }
        }
        else//�Q�X�g�̃\�P�b�g�̐ڑ��ɐ��������Ƃ�
        {
            if (emptyIndex < 0)//����I�[�o�[
            {
                ConsoleFunc::Write("TCP:");
                ConsoleFunc::WriteEndl("����I�[�o�[�ł��B���߂�Ȃ���");
            }
            else//���X�g�ɋ󂫂����݂���Ƃ�
            {
                return true;
                //�N���C�A���g�\�P�b�g���\�P�b�g���X�g�ɓo�^
                //RegisterList(emptyIndex, newData);
            }
        }
    }
    return false;
}

//�N���C�A���g��TCP�ʐM���g�p���ăf�[�^�𑗐M
void CommunicateTCPServer::Send(SOCKET receiverSocket, char* data, int size)
{
    send(receiverSocket, data, size, 0);
}

//�N���C�A���g����TCP�ʐM���g�p���ăf�[�^����M
bool CommunicateTCPServer::Recieve(SOCKET senderSocket)
{
    int recieveCount{ 0 };

    recieveCount = recv(senderSocket, (char*)&buffer, sizeof(buffer), 0);

    if (recieveCount > 0)
    {
        ConsoleFunc::Write("TCP:");
        //CommandFromData();
        return true;
    }
    return false;
}

bool CommunicateTCPServer::RecieveOnSelect(SOCKET senderSocket)
{
    //if (!completeInitialize)return false;

    action = {};
    memset(buffer, '\0', sizeof(buffer));
    int recieveCount{ 0 };

    struct timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = 0;
    fd_set socketTmp;
    memcpy(&socketTmp, &clientList, sizeof(clientList));

    //�ω����������\�P�b�g�̐����������Ă���
    int variableSocketCount = 0;
    variableSocketCount = select(0, &socketTmp, NULL, NULL, &tm);
    if (variableSocketCount > 0)
    {
        for (int i = 0; i < 4; i++)
        {
            recieveCount = recv(senderSocket, (char*)&buffer, sizeof(buffer), 0);

            if (recieveCount > 0)
            {
                ConsoleFunc::Write("TCP:");
                //CommandFromData();
                return true;
            }
        }
    }
    return false;}

void CommunicateTCPServer::SetList(SOCKET* sock)
{
    FD_SET(*sock, &clientList);
}
