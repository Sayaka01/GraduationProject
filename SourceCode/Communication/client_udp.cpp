#include "Stdafx.h"
#include "client_udp.h"
#include <ws2tcpip.h>

CommunicateUDPClient::~CommunicateUDPClient()
{
}

//void CommunicateUDPClient::InitializeUDPServer(int PortIndex)
//{
//    addrinfo* addrInfo = NULL;
//    addrinfo tmpAddr;
//    ZeroMemory(&tmpAddr, sizeof(addrinfo));
//    ZeroMemory(&clientListUDP, sizeof(clientListUDP));
//    //�ݒ肷�����ݒ�
//    sockaddr_in addrUDP;
//    tmpAddr.ai_flags = 0;
//    tmpAddr.ai_family = AF_INET;
//    tmpAddr.ai_socktype = SOCK_DGRAM;
//    tmpAddr.ai_protocol = 0;
//    addrUDP.sin_family = AF_INET;
//    addrUDP.sin_port = htons(_Port + PortIndex);
//    addrUDP.sin_addr.S_un.S_addr = INADDR_ANY;
//
//    sockaddr_in ownAddr;
//    ownAddr = GetHostNameAndAddress();
//
//    //if (getaddrinfo(inputHostAddress, port, &tmpAddr, &addrInfo) != 0)
//    //{
//    //    Fail("�A�h���X�̎擾�Ɏ��s���܂���");
//    //    return;
//    //}
//    //
//    //serverAddr = *((sockaddr_in*)addrInfo->ai_addr);
//
//    //�\�P�b�g�̐���
//    clientSockUDP = socket(AF_INET, SOCK_DGRAM, 0);
//    if (clientSockUDP == INVALID_SOCKET)
//    {
//        //���s
//        Fail("�\�P�b�g�̐����Ɏ��s���܂���");
//        return;
//    }
//
//    //�\�P�b�g�Ǝ󂯓�������֘A�t����
//    if (bind(clientSockUDP, (struct sockaddr*)&addrUDP, sizeof(addrUDP)) != 0)
//    {
//        Fail("�\�P�b�g�Ǝ󂯓�����̊֘A�t���Ɏ��s���܂���");
//        return;
//    }
//
//    //�m���u���b�L���O�ɐݒ�
//    u_long val = 1;
//    ioctlsocket(clientSockUDP, FIONBIO, &val);
//
//    Console::WriteEndl("�T�[�o�[������ɋN�����܂����iUDP�j", Console::Blue);
//
//    //clients.data[0].socketUDP = serverSockUDP;
//    //clients.data[0].addrUDP = ownAddr;
//    //std::cout << "�|�[�g�ԍ��iUDP�j" << clients.data[0].addrUDP.sin_port << std::endl;
//
//    //JoinCommand joinCom;
//    //joinCom.actionType = (char)ActionCommandType::Join;
//    //SendUDP((char*)&joinCom, sizeof(JoinCommand));
//
//    //RecieveUDP();
//}

//UDP�ʐM�̏����ݒ�
bool CommunicateUDPClient::Initialize(int port, char* hostAddress)
{
    CommunicateBaseClient::Initialize(port,hostAddress);

    addrinfo* addrInfo = NULL;
    addrinfo tmpAddr;
    ZeroMemory(&tmpAddr, sizeof(addrinfo));
    ZeroMemory(&clientList, sizeof(clientList));
    //�ݒ肷�����ݒ�
    sockaddr_in addrUDP;
    tmpAddr.ai_flags = 0;
    tmpAddr.ai_family = AF_INET;
    tmpAddr.ai_socktype = SOCK_DGRAM;
    tmpAddr.ai_protocol = 0;
    //addrUDP.sin_family = AF_INET;
    //addrUDP.sin_port = htons(_Port + PortIndex);
    //addrUDP.sin_addr.S_un.S_addr = INADDR_ANY;
    std::string portIndex = std::to_string(port);

    //���̃A�h���X��host�֑��M
    //�����������ۑ����Ă����悤�ɂ���

    if (getaddrinfo(inputHostAddress, portIndex.c_str(), &tmpAddr, &addrInfo) != 0)
    {
        Fail("udp:�A�h���X�̎擾�Ɏ��s���܂���");
        return false;
    }

    addrUDP = *((sockaddr_in*)addrInfo->ai_addr);

    //�\�P�b�g�̐���
    ownSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
    if (ownSocket == INVALID_SOCKET)
    {
        //���s
        Fail("udp:�\�P�b�g�̐����Ɏ��s���܂���");
        return false;
    }

    //�\�P�b�g�Ǝ󂯓�������֘A�t����
    //if (bind(ownSocket, (struct sockaddr*)&addrUDP, sizeof(addrUDP)) != 0)
    //{
    //    Fail("udp:�\�P�b�g�Ǝ󂯓�����̊֘A�t���Ɏ��s���܂���");
    //    return;
    //}

    //�m���u���b�L���O�ɐݒ�
    u_long val = 1;
    ioctlsocket(ownSocket, FIONBIO, &val);

    ConsoleFunc::WriteEndl("�󂯓�����̊֘A�t���ɐ������܂����B(UDP)", ConsoleFunc::Blue);
    completeInitialize = true;

    //JoinCommand joinCom;
    //joinCom.actionType = (char)ActionCommandType::Join;
    //SendUDP((char*)&joinCom, sizeof(JoinCommand));

    //RecieveUDP();
    return true;
}

//�\�P�b�g��
void CommunicateUDPClient::Finalize()
{
    int close = closesocket(ownSocket);
    if (close != 0)
    {
        Fail("�I�������Ɏ��s���܂��� �G���[�R�[�h�F");
    }
}

void CommunicateUDPClient::Send(sockaddr_in recverAddr,char* data, int size)
{
    sendto(ownSocket, data, size, 0, (struct sockaddr*)(&recverAddr), sizeof(sockaddr_in));

}

bool CommunicateUDPClient::Recieve()
{
    //if (!completeInitialize)return false;

    int recieveCount{ 0 };
    int size = sizeof(sockaddr_in);
    sockaddr_in fromAddr;
    recieveCount = recvfrom(ownSocket, (char*)&buffer, sizeof(buffer), 0, (struct sockaddr*)&fromAddr, &size);
    if (recieveCount > 0)
    //if (recv(ownSocket, (char*)&buffer, sizeof(buffer), 0) > 0)
    {
        ConsoleFunc::Write("UDPc:");

        //�����Ă����f�[�^���R�}���h�ɕϊ�
        //CommandFromData();
        return true;
    }
    return false;
}

void CommunicateUDPClient::SendChatMessage(char chatMessage[128],sockaddr_in recieverAddr)
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);

    Send(recieverAddr,(char*)&chatCom, sizeof(ChatCommand));
}
