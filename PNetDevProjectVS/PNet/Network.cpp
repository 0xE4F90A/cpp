//Network.cpp
#include "Network.h"
#include <iostream>

bool PNet::Network::Initialize()
{
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);

	//Winsock APIの起動に失敗したら
	if (result != 0)
	{
		std::cerr << "Winsock API の起動に失敗しました。" << std::endl;
		return false;
	}

	//受信したバージョンが要求したバージョンと一致しない場合(2.2)
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
	{
		std::cerr << "Winsock API dll の使用可能なバージョンが見つかりませんでした。" << std::endl;
		return false;
	}

	return true;
}

void PNet::Network::Shutdown()
{
	//APIクリーンアップ
	WSACleanup();
}
