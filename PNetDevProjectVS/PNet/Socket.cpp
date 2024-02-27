//Socket.cpp

#include "Socket.h"
#include <assert.h>
#include <iostream>

namespace PNet
{
	//Socket�N���X�̃R���X�g���N�^
	Socket::Socket(IPVersion ipversion, SocketHandle handle)
		:ipversion(ipversion), handle(handle)
	{
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6); //IP�o�[�W������IPv4�܂���IPv6�ł��邱�Ƃ��m�F
	}

	//�\�P�b�g���쐬����֐�
	PResult Socket::Create()
	{
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6); //IP�o�[�W������IPv4�܂���IPv6�ł��邱�Ƃ��m�F

		if (handle != INVALID_SOCKET)
		{
			return PResult::P_GenericError; //�G���[: ���ɗL���ȃ\�P�b�g�����݂���ꍇ
		}

		handle = socket((ipversion == IPVersion::IPv4) ? AF_INET : AF_INET6, SOCK_STREAM, IPPROTO_TCP); //�\�P�b�g���쐬

		if (handle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError; //�G���[: �\�P�b�g�̍쐬�Ɏ��s
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != PResult::P_Success)
		{
			return PResult::P_GenericError; //�G���[: TCP_NoDelay�I�v�V�����̐ݒ�Ɏ��s
		}

		return PResult::P_Success; //����
	}

	//�\�P�b�g�����֐�
	PResult Socket::Close()
	{
		if (handle == INVALID_SOCKET)
		{
			return PResult::P_GenericError; //�G���[: �����ȃ\�P�b�g����悤�Ƃ��Ă���
		}

		int result = closesocket(handle);
		if (result != 0) //�G���[: �\�P�b�g�̃N���[�Y�Ɏ��s
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		handle = INVALID_SOCKET;
		return PResult::P_Success; //����
	}

	//�\�P�b�g�ɃA�h���X���o�C���h����֐�
	PResult Socket::Bind(IPEndpoint endpoint)
	{
		assert(ipversion == endpoint.GetIPVersion()); //IP�o�[�W��������v���邱�Ƃ��m�F

		if (ipversion == IPVersion::IPv4)
		{
			sockaddr_in addr = endpoint.GetSockaddrIPv4();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
			if (result != 0) //�G���[: �o�C���h�Ɏ��s
			{
				int error = WSAGetLastError();
				return PResult::P_GenericError;
			}
		}
		else //IPv6
		{
			sockaddr_in6 addr = endpoint.GetSockaddrIPv6();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in6));
			if (result != 0) //�G���[: �o�C���h�Ɏ��s
			{
				int error = WSAGetLastError();
				return PResult::P_GenericError;
			}
		}

		return PResult::P_Success; //����
	}

	//�ڑ��҂���Ԃɂ���֐�
	PResult Socket::Listen(IPEndpoint endpoint, int backlog)
	{
		if (ipversion == IPVersion::IPv6)
		{
			if (SetSocketOption(SocketOption::IPV6_Only, FALSE) != PResult::P_Success)
			{
				return PResult::P_GenericError; //�G���[: IPV6_Only�I�v�V�����̐ݒ�Ɏ��s
			}
		}


		if (Bind(endpoint) != PResult::P_Success)
		{
			return PResult::P_GenericError; //�G���[: �o�C���h�Ɏ��s
		}

		int result = listen(handle, backlog);
		if (result != 0) //�G���[: ���X�j���O�Ɏ��s
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		return PResult::P_Success; //����
	}

	//�ڑ����󂯓����֐�
	PResult Socket::Accept(Socket& outSocket)
	{
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6); //IP�o�[�W������IPv4�܂���IPv6�ł��邱�Ƃ��m�F

		if (ipversion == IPVersion::IPv4)
		{
			sockaddr_in addr = {};
			int len = sizeof(sockaddr_in);
			SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
			if (acceptedConnectionHandle == INVALID_SOCKET)
			{
				int error = WSAGetLastError();
				return PResult::P_GenericError; //�G���[: �ڑ��̎󂯓���Ɏ��s
			}
			IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
			std::cout << "�V�����ڑ���ҋ@���Ă��܂�..." << std::endl;
			newConnectionEndpoint.Print();
			outSocket = Socket(IPVersion::IPv4, acceptedConnectionHandle);
		}
		else //IPv6
		{
			sockaddr_in6 addr = {};
			int len = sizeof(sockaddr_in6);
			SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
			if (acceptedConnectionHandle == INVALID_SOCKET)
			{
				int error = WSAGetLastError();
				return PResult::P_GenericError; //�G���[: �ڑ��̎󂯓���Ɏ��s
			}
			IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
			std::cout << "�V�����ڑ���ҋ@���Ă��܂�..." << std::endl;
			newConnectionEndpoint.Print();
			outSocket = Socket(IPVersion::IPv6, acceptedConnectionHandle);
		}

		return PResult::P_Success; //����
	}

	//�ڑ����m������֐�
	PResult Socket::Connect(IPEndpoint endpoint)
	{
		assert(ipversion == endpoint.GetIPVersion()); //IP�o�[�W��������v���邱�Ƃ��m�F

		int result = 0;
		if (ipversion == IPVersion::IPv4)
		{
			sockaddr_in addr = endpoint.GetSockaddrIPv4();
			result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
		}
		else //IPv6
		{
			sockaddr_in6 addr = endpoint.GetSockaddrIPv6();
			result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in6));
		}
		if (result != 0) //�G���[: �ڑ��Ɏ��s
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}
		return PResult::P_Success; //����
	}

	//�����́A
	//====================//
	//�f�[�^�𑗐M����֐�//
	//====================//

	PResult Socket::Send(const void* data, int numberOfBytes, int& bytesSent)
	{
		bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);

		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError; //�G���[: ���M�Ɏ��s
		}

		return PResult::P_Success; //����
	}

	//�f�[�^����M����֐�
	PResult Socket::Recv(void* destination, int numberOfBytes, int& bytesReceived)
	{
		bytesReceived = recv(handle, (char*)destination, numberOfBytes, NULL);

		if (bytesReceived == 0) //�ڑ�������ɃN���[�Y���ꂽ�ꍇ
		{
			return PResult::P_GenericError; //�G���[: ��M�����f�[�^���Ȃ�
		}

		if (bytesReceived == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError; //�G���[: ��M�Ɏ��s
		}

		return PResult::P_Success; //����
	}

	//�f�[�^�����ׂđ��M����֐�
	PResult Socket::SendAll(const void* data, int numberOfBytes)
	{

		int totalBytesSent = 0;

		while (totalBytesSent < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytesSent;
			int bytesSent = 0;
			char* bufferOffset = (char*)data + totalBytesSent;
			PResult result = Send(bufferOffset, bytesRemaining, bytesSent);
			if (result != PResult::P_Success)
			{
				return PResult::P_GenericError; //�G���[: ���M�Ɏ��s
			}
			totalBytesSent += bytesSent;
		}

		return PResult::P_Success; //����
	}

	//�f�[�^�����ׂĎ�M����֐�
	PResult Socket::RecvAll(void* destination, int numberOfBytes)
	{
		int totalBytesReceived = 0;

		while (totalBytesReceived < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytesReceived;
			int bytesReceived = 0;
			char* bufferOffset = (char*)destination + totalBytesReceived;
			PResult result = Recv(bufferOffset, bytesRemaining, bytesReceived);
			if (result != PResult::P_Success)
			{
				return PResult::P_GenericError; //�G���[: ��M�Ɏ��s
			}
			totalBytesReceived += bytesReceived;
		}

		return PResult::P_Success; //����
	}

	//�p�P�b�g�𑗐M����֐�
	PResult Socket::Send(Packet& packet)
	{
		uint16_t encodedPacketSize = htons(packet.buffer.size());
		PResult result = SendAll(&encodedPacketSize, sizeof(uint16_t));
		if (result != PResult::P_Success)
			return PResult::P_GenericError;

		result = SendAll(packet.buffer.data(), packet.buffer.size());
		if (result != PResult::P_Success)
			return PResult::P_GenericError;

		return PResult::P_Success; //����
	}

	//�p�P�b�g����M����֐�
	PResult Socket::Recv(Packet& packet)
	{
		packet.Clear();

		uint16_t encodedSize = 0;
		PResult result = RecvAll(&encodedSize, sizeof(uint16_t));
		if (result != PResult::P_Success)
			return PResult::P_GenericError;

		uint16_t bufferSize = ntohs(encodedSize);

		if (bufferSize > PNet::g_MaxPacketSize)
			return PResult::P_GenericError; //�G���[: ��M�����p�P�b�g�T�C�Y���傫������

		packet.buffer.resize(bufferSize);
		result = RecvAll(&packet.buffer[0], bufferSize);
		if (result != PResult::P_Success)
			return PResult::P_GenericError; //�G���[: ��M�Ɏ��s

		return PResult::P_Success; //����
	}


	//�\�P�b�g�̃n���h�����擾����֐�
	SocketHandle Socket::GetHandle()
	{
		return handle;
	}

	//�\�P�b�g��IP�o�[�W�������擾����֐�
	IPVersion Socket::GetIPVersion()
	{
		return ipversion;
	}

	//�\�P�b�g�I�v�V������ݒ肷��֐�
	PResult Socket::SetSocketOption(SocketOption option, BOOL value)
	{
		int result = 0;
		switch (option)
		{
		case SocketOption::TCP_NoDelay:
			result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
			break;
		case SocketOption::IPV6_Only:
			result = setsockopt(handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&value, sizeof(value));
			break;
		default:
			return PResult::P_GenericError; //�G���[: �����ȃI�v�V����
		}

		if (result != 0) //�G���[: �\�P�b�g�I�v�V�����̐ݒ�Ɏ��s
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		return PResult::P_Success; //����
	}
}
