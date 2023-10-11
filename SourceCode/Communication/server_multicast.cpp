#include "Stdafx.h"
#include "server_multicast.h"

CommunicateMultiCastServer::~CommunicateMultiCastServer()
{
	Finalize();
}

bool CommunicateMultiCastServer::Initialize(int port)
{
    CommunicateBaseServer::Initialize(port);

    //�\�P�b�g�̐���
    ownSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (ownSocket == INVALID_SOCKET)
    {
        Fail("�\�P�b�g�̐����Ɏ��s���܂���");
        return false;
    }

    //���̐ݒ�
	ownAddress.sin_family = AF_INET;
    ownAddress.sin_port = htons(port);
    ownAddress.sin_addr.S_un.S_addr = INADDR_ANY;

	localAddress = GetLocalAddress();

	// IP�A�h���X�𕶎��񂩂琔�l�ɕϊ�
	if (!ChangeStringToLong(ownAddress.sin_family, multicast_ip, ownAddress.sin_addr.S_un.S_addr))
	{
		Fail("�}���`�L���X�g��IP�A�h���X�̕ϊ��Ɏ��s���܂���:");
		return false;
	}
	// UDP�}���`�L���X�g�\�P�b�g�I�v�V�����Z�b�g
	LONG localAddr = localAddress.sin_addr.S_un.S_addr;
	int set_opt_result = setsockopt(ownSocket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localAddr, sizeof(localAddr));
	// �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[����
	if (set_opt_result != 0)
	{
		Fail("�}���`�L���X�g�\�P�b�g�I�v�V�����Z�b�g�Ɏ��s���܂���:");
		return false;
	}
	// UDP�}���`�L���X�gTTL�Z�b�g
	// 0:�����z�X�g�ɐ���
	// 1:�����T�u�l�b�g�ɐ���
	// 32:�����T�C�g�ɐ���
	// 64:�����n��ɐ���
	// 128:�����嗤�ɐ���
	// 225:�z�M�͈͓��Ő�������Ȃ�
	int TTL = 1;

	// UDP�}���`�L���X�gTTL�\�P�b�g�I�v�V�����Z�b�g
	int set_ttl_result = setsockopt(ownSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&TTL, sizeof(TTL));
	// �\�P�b�g�I�v�V�����Z�b�g���s���̃G���[����
	if (set_ttl_result != 0)
	{
		Fail("UDP�}���`�L���X�gTTL�\�P�b�g�I�v�V�����Z�b�g�Ɏ��s���܂���");
		return false;
	}

	ConsoleFunc::WriteEndl("�}���`�L���X�g�T�[�o�[�̐ݒ�ɐ������܂����B", ConsoleFunc::Blue);
	completeInitialize = true;
	return true;
}

void CommunicateMultiCastServer::Finalize()
{
	CommunicateBaseServer::Finalize();
}

void CommunicateMultiCastServer::Send(char* data, int size)
{
	// �}���`�L���X�g�A�h���X������Ɏw�肵�ă��b�Z�[�W�𑗐M
	sendto(ownSocket, data, size, 0, (struct sockaddr*)&ownAddress, sizeof(ownAddress));
}
