#pragma once
#include <WinSock2.h>//windows.h���O�ɋL�q����

#include "server.h"

//*********************************************************************************************
//  �}���`�L���X�g�ʐM�p�T�[�o�[�iSender�j
//*********************************************************************************************

class CommunicateMultiCastServer :public CommunicateBaseServer
{
public:

    CommunicateMultiCastServer() = default;
    ~CommunicateMultiCastServer()override;

    //UDP�ʐM�̏����ݒ�
    bool Initialize(int port)override;
    //�\�P�b�g�̏I������
    void Finalize()override;

    //�N���C�A���g��UDP�ʐM���g�p���ăf�[�^�𑗐M
    void Send(char* data, int size);

private:
    sockaddr_in localAddress;
    std::string multicast_ip = "224.10.1.1";
};