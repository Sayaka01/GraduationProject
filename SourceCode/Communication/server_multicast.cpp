#include "Stdafx.h"
#include "server_multicast.h"

CommunicateMultiCastServer::~CommunicateMultiCastServer()
{
	Finalize();
}

bool CommunicateMultiCastServer::Initialize(int port)
{
    CommunicateBaseServer::Initialize(port);

    //ソケットの生成
    ownSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (ownSocket == INVALID_SOCKET)
    {
        Fail("ソケットの生成に失敗しました");
        return false;
    }

    //情報の設定
	ownAddress.sin_family = AF_INET;
    ownAddress.sin_port = htons(port);
    ownAddress.sin_addr.S_un.S_addr = INADDR_ANY;

	localAddress = GetLocalAddress();

	// IPアドレスを文字列から数値に変換
	if (!ChangeStringToLong(ownAddress.sin_family, multicast_ip, ownAddress.sin_addr.S_un.S_addr))
	{
		Fail("マルチキャストのIPアドレスの変換に失敗しました:");
		return false;
	}
	// UDPマルチキャストソケットオプションセット
	LONG localAddr = localAddress.sin_addr.S_un.S_addr;
	int set_opt_result = setsockopt(ownSocket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localAddr, sizeof(localAddr));
	// ソケットオプションセット失敗時のエラー処理
	if (set_opt_result != 0)
	{
		Fail("マルチキャストソケットオプションセットに失敗しました:");
		return false;
	}
	// UDPマルチキャストTTLセット
	// 0:同じホストに制限
	// 1:同じサブネットに制限
	// 32:同じサイトに制限
	// 64:同じ地域に制限
	// 128:同じ大陸に制限
	// 225:配信範囲内で制限されない
	int TTL = 1;

	// UDPマルチキャストTTLソケットオプションセット
	int set_ttl_result = setsockopt(ownSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&TTL, sizeof(TTL));
	// ソケットオプションセット失敗時のエラー処理
	if (set_ttl_result != 0)
	{
		Fail("UDPマルチキャストTTLソケットオプションセットに失敗しました");
		return false;
	}

	ConsoleFunc::WriteEndl("マルチキャストサーバーの設定に成功しました。", ConsoleFunc::Blue);
	completeInitialize = true;
	return true;
}

void CommunicateMultiCastServer::Finalize()
{
	CommunicateBaseServer::Finalize();
}

void CommunicateMultiCastServer::Send(char* data, int size)
{
	// マルチキャストアドレスを宛先に指定してメッセージを送信
	sendto(ownSocket, data, size, 0, (struct sockaddr*)&ownAddress, sizeof(ownAddress));
}
