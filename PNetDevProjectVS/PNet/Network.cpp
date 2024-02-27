//Network.cpp
#include "Network.h"
#include <iostream>

bool PNet::Network::Initialize()
{
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);

	//Winsock API�̋N���Ɏ��s������
	if (result != 0)
	{
		std::cerr << "Winsock API �̋N���Ɏ��s���܂����B" << std::endl;
		return false;
	}

	//��M�����o�[�W�������v�������o�[�W�����ƈ�v���Ȃ��ꍇ(2.2)
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
	{
		std::cerr << "Winsock API dll �̎g�p�\�ȃo�[�W������������܂���ł����B" << std::endl;
		return false;
	}

	return true;
}

void PNet::Network::Shutdown()
{
	//API�N���[���A�b�v
	WSACleanup();
}
