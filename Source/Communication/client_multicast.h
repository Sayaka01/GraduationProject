#pragma once
#include <WinSock2.h>//windows.h���O�ɋL�q����
#include <Ws2tcpip.h>
#include "client.h"

//*********************************************************************************************
//  �}���`�L���X�g�ʐM�p�N���C�A���g�iReciever�j
//*********************************************************************************************

class CommunicateMultiCastClient :public CommunicateBaseClient
{
public:
    // �R���X�g���N�^
    CommunicateMultiCastClient() = default;
    // �f�X�g���N�^
    ~CommunicateMultiCastClient();

    // �}���`�L���X�g�ʐM�̏����ݒ�
    bool Initialize(int port) override;


    // �I������
    void Finalize() override;

    // �f�[�^�̎�M����
    bool Recieve();

    sockaddr_in& GetFromAddress() { return fromAddr; }

private:
    SOCKET ownSocket{};//�����̃\�P�b�g
    std::string multicast_ip = "224.10.1.1";//�}���`�L���X�gIP�A�h���X
    // �}���`�L���X�g���N�G�X�g
    ip_mreq MulticastRequest;

    //������PC�̖��O��IP�A�h���X���擾
//true:�擾�ɐ�������
    sockaddr_in GetLocalAddress();

    sockaddr_in localAddress;
    //�f�[�^�̑��M����ꎞ�I�ɕۑ�
    sockaddr_in fromAddr;

};
