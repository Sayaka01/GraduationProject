#pragma once
#include <WinSock2.h>//windows.h���O�ɋL�q����

#include "server.h"
#include "../System/console.h"

//*********************************************************************************************
//  ���j�L���X�g�ʐM�p UDP �T�[�o�[
//*********************************************************************************************

class CommunicateUDPServer:public CommunicateBaseServer
{
public:

    CommunicateUDPServer() = default;
    ~CommunicateUDPServer()override;

    //UDP�ʐM�̏����ݒ�
    bool Initialize(int port)override;
    //�\�P�b�g�̏I������
    void Finalize()override;

    //�N���C�A���g��UDP�ʐM���g�p���ăf�[�^�𑗐M
    void Send(sockaddr_in receiverAddr, char* data, int size);
    //�N���C�A���g����UDP�ʐM���g�p���ăf�[�^����M
    sockaddr_in Recieve();

    char GetAction() { return action; }
    char* GetBuffer() { return buffer; }

    sockaddr_in& GetSocketAddress() { return addrFromName; }

private:
    sockaddr_in addrFromName;
    fd_set serverList;//�m���u���b�L���O�ݒ莞�̃T�[�o�[���X�g
    fd_set clientList;//�m���u���b�L���O�ݒ莞�̃N���C�A���g���X�g
};
