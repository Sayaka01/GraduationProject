#include "Stdafx.h"
#include "client_tcp.h"
#include <process.h>
#include <ws2tcpip.h>

CommunicateTCPClient::~CommunicateTCPClient()
{
}

//TCP�ʐM�̏����ݒ�
bool CommunicateTCPClient::Initialize(int port, char* hostAddress)
{
    CommunicateBaseClient::Initialize(port,hostAddress);
    //�ڑ��ɕK�v�ȏ���ݒ�
    struct sockaddr_in addr;
    //addr.sin_family = AF_INET;
    //addr.sin_port = htons(port);
    //addr.sin_addr.S_un.S_addr = hostAddress;
    addrinfo hints;
    addrinfo* pAddrInfo = NULL;
    ZeroMemory(&hints, sizeof(addrinfo));

    // �擾�������ݒ�
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    std::string portStr = std::to_string(port);
    if (getaddrinfo(hostAddress, (char*)portStr.c_str(), &hints, &pAddrInfo) != 0)
    {
        Fail("�A�h���X�̐����Ɏ��s���܂���");
        return false;
    }
    addr = *((sockaddr_in*)pAddrInfo->ai_addr);

    //�\�P�b�g�̐���
    ownSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ownSocket == INVALID_SOCKET)
    {
        //���s
        Fail("�\�P�b�g�̐����Ɏ��s���܂���");
        return false;
    }

    //���X�g�̏�����
    FD_ZERO(&clientList);

    //�T�[�o�[�ւ̐ڑ�
    int connection = connect(ownSocket, (struct sockaddr*)(&addr), sizeof(addr));
    if (connection != 0)
    {
        Fail("�ڑ��Ɏ��s���܂���");
        return false;
    }
    FD_SET(ownSocket, &clientList);

    ConsoleFunc::WriteEndl("�T�[�o�[�ɐڑ����܂����B(TCP)", ConsoleFunc::Blue);
    //CommunicateCommand com;
    //com.actionType = static_cast<int>(ActionCommandType::Exit);
    //UpdateCommand upCom{};
    //upCom.actionType = static_cast<char>(ActionCommandType::Update);
    //upCom.personType = static_cast<char>(PersonType::Player);
    //upCom.pos = { 1.5f,6.4f,3.3f };
    //strcpy_s(info.name, sizeof(playerName), playerName);
    //JoinCommand joinCom;
    //joinCom.actionType = (char)ActionCommandType::Join;
    //SendTCP((char*)&joinCom,sizeof(JoinCommand));

    struct sockaddr_in localAddress {};
    socklen_t addressLength = sizeof(localAddress);
    if (getsockname(ownSocket, reinterpret_cast<struct sockaddr*>(&localAddress), &addressLength) < 0) {
        std::cout << "�\�P�b�g���̎擾���ɃG���[���������܂���" << std::endl;
        //closesocket(ownSocket);
    }


    char ipBuffer[INET_ADDRSTRLEN];
    const char* ipAddress = inet_ntop(AF_INET, &(localAddress.sin_addr), ipBuffer, INET_ADDRSTRLEN);
    if (ipAddress == nullptr) {
        std::cout << "IP�A�h���X�̕ϊ����ɃG���[���������܂���" << std::endl;
    }

    printf("%d.%d.%d.%d",
        localAddress.sin_addr.S_un.S_un_b.s_b1,
        localAddress.sin_addr.S_un.S_un_b.s_b2,
        localAddress.sin_addr.S_un.S_un_b.s_b3,
        localAddress.sin_addr.S_un.S_un_b.s_b4);


    completeInitialize = true;
    return true;
}

void CommunicateTCPClient::Finalize()
{
    int close = closesocket(ownSocket);
    if (close != 0)
    {
        Fail("�I�������Ɏ��s���܂��� �G���[�R�[�h�F");
    }
}

//�T�[�o�[��TCP�ʐM���g�p���ăf�[�^�𑗐M
void CommunicateTCPClient::Send(char* data, int size)
{
    send(ownSocket, data, size, 0);
}

//�T�[�o�[����TCP�ʐM���g�p���ăf�[�^����M
bool CommunicateTCPClient::Recieve()
{
    //if (!completeInitialize)return false;

    if (recv(ownSocket, (char*)&buffer, sizeof(buffer), 0) > 0)
    {
        ConsoleFunc::Write("TCP:");
        //�����Ă����f�[�^���R�}���h�ɕϊ�
        //CommandFromData();
        return true;
    }
    return false;
}

//select�֐����g�p���ă\�P�b�g�ɕω������������̂݃T�[�o�[����TCP�ʐM���g�p���ăf�[�^����M
bool CommunicateTCPClient::RecieveOnSelect()
{
    //if (!completeInitialize)return false;

    //action = {};
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
        recieveCount = recv(ownSocket, (char*)&buffer, sizeof(buffer), 0);

        if (recieveCount > 0)
        {
            ConsoleFunc::Write("TCP:");
            //�����Ă����f�[�^���R�}���h�ɕϊ�
            //CommandFromData();
            return true;
        }
    }
    return false;
}
