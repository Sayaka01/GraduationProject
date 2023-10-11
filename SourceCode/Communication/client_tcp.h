#pragma once

#include <WinSock2.h>//windows.h���O�ɋL�q����

#include "client.h"

//*********************************************************************************************
//  ���j�L���X�g�ʐM�p TCP �N���C�A���g
//*********************************************************************************************

class CommunicateTCPClient:public CommunicateBaseClient
{
public:
    CommunicateTCPClient() = default;
    ~CommunicateTCPClient();

    //TCP�ʐM�̏����ݒ�
    bool Initialize(int port,char* hostAddress) override;

    //�\�P�b�g�̏I������
    void Finalize() override;

    //�T�[�o�[��TCP�ʐM���g�p���ăf�[�^�𑗐M
    // data :���肽���f�[�^�@size :���肽���f�[�^�̃T�C�Y(�\���̂̃T�C�Y)
    void Send(char* data, int size);

    //�T�[�o�[����TCP�ʐM���g�p���ăf�[�^����M
    bool Recieve();

    //select�֐����g�p���ă\�P�b�g�ɕω������������̂݃T�[�o�[����TCP�ʐM���g�p���ăf�[�^����M
    bool RecieveOnSelect();


    //const ComputerData& GetJoinedData(const int index) { return joinedObjectData[index]; }
private:
    SOCKET ownSocket{};//�����̃\�P�b�g
    fd_set clientList{};//�m���u���b�L���O�ݒ莞�̃T�[�o�[���X�g

};