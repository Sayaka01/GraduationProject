#pragma once
#include <WinSock2.h>//windows.h���O�ɋL�q����

#include "client.h"

//*********************************************************************************************
//  ���j�L���X�g�ʐM�p UDP �N���C�A���g
//*********************************************************************************************

class CommunicateUDPClient:public CommunicateBaseClient
{
public:
    CommunicateUDPClient() = default;
    ~CommunicateUDPClient();

    //UDP�ʐM�̏����ݒ�
   // void InitializeServer(int PortIndex);
    bool Initialize(int port, char* hostAddress) override;

    //�\�P�b�g�̏I������
    void Finalize() override;

    //�T�[�o�[��UDP�ʐM���g�p���ăf�[�^�𑗐M
    // data :���肽���f�[�^�@size :���肽���f�[�^�̃T�C�Y(�\���̂̃T�C�Y)
    void Send(sockaddr_in recverAddr,char* data, int size);

    //�T�[�o�[����UDP�ʐM���g�p���ăf�[�^����M
    bool Recieve();

    //�`���b�g���b�Z�[�W���T�[�o�[�֑��M
    // chatMessage :���肽�����b�Z�[�W
    void SendChatMessage(char chatMessage[128],sockaddr_in recieverAddr);

private:
    SOCKET ownSocket{};//�����̃\�P�b�g
    fd_set clientList{};//�m���u���b�L���O�ݒ莞�̃T�[�o�[���X�g
};