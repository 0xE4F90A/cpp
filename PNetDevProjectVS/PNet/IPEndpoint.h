//IPEndpoint.h
#pragma once
#include "IPVersion.h"
#include <string>
#include <vector>
#include <WS2tcpip.h>

namespace PNet
{
	class IPEndpoint
	{
	public:
		//�R���X�g���N�^: IP�A�h���X�ƃ|�[�g�ԍ��������Ɏ��
		IPEndpoint(const char* ip, unsigned short port);

		//�R���X�g���N�^: sockaddr �\���̂������Ɏ��
		IPEndpoint(sockaddr* addr);

		//IP�o�[�W�������擾���郁�\�b�h
		IPVersion GetIPVersion();

		//IP�A�h���X�̃o�C�g�\�����擾���郁�\�b�h
		std::vector<uint8_t> GetIPBytes();

		//�z�X�g�����擾���郁�\�b�h
		std::string GetHostname();

		//IP�A�h���X�̕�����\�����擾���郁�\�b�h
		std::string GetIPString();

		//�|�[�g�ԍ����擾���郁�\�b�h
		unsigned short GetPort();

		//IPv4 �� sockaddr_in �\���̂��擾���郁�\�b�h
		sockaddr_in GetSockaddrIPv4();

		//IPv6 �� sockaddr_in6 �\���̂��擾���郁�\�b�h
		sockaddr_in6 GetSockaddrIPv6();

		//�G���h�|�C���g�̏ڍׂ��o�͂��郁�\�b�h
		void Print();

	private:
		//IP�o�[�W����
		IPVersion ipversion = IPVersion::Unknown;

		//�z�X�g��
		std::string hostname = "";

		//IP�A�h���X�̕�����\��
		std::string ip_string = "";

		//IP�A�h���X�̃o�C�g�\��
		std::vector<uint8_t> ip_bytes;

		//�|�[�g�ԍ�
		unsigned short port = 0;
	};
}
