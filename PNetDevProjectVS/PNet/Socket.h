//Socket.h
#pragma once
#include "SocketHandle.h"
#include "PResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"
#include "Constants.h"
#include "Packet.h"

namespace PNet
{
	class Socket
	{
	public:
		//�R���X�g���N�^
		Socket( IPVersion ipversion = IPVersion::IPv4,
				SocketHandle handle = INVALID_SOCKET);

		//�\�P�b�g�̍쐬
		PResult Create();

		//�\�P�b�g�̃N���[�Y
		PResult Close();

		//�G���h�|�C���g�ւ̃o�C���h
		PResult Bind(IPEndpoint endpoint);

		//�G���h�|�C���g�ł̃��b�X��
		PResult Listen(IPEndpoint endpoint, int backlog = 5);

		//�\�P�b�g�̎󂯓���
		PResult Accept(Socket& outSocket);

		//�G���h�|�C���g�ւ̐ڑ�
		PResult Connect(IPEndpoint endpoint);

		//�f�[�^�̑��M
		PResult Send(const void* data, int numberOfBytes, int& bytesSent);

		//�f�[�^�̎�M
		PResult Recv(void* destination, int numberOfBytes, int& bytesReceived);

		//�S�f�[�^�̑��M
		PResult SendAll(const void* data, int numberOfBytes);

		//�S�f�[�^�̎�M
		PResult RecvAll(void* destination, int numberOfBytes);

		//�p�P�b�g�̑��M
		PResult Send(Packet& packet);

		//�p�P�b�g�̎�M
		PResult Recv(Packet& packet);

		//�\�P�b�g�n���h���̎擾
		SocketHandle GetHandle();

		//IP�o�[�W�����̎擾
		IPVersion GetIPVersion();

	private:
		//�\�P�b�g�I�v�V�����̐ݒ�
		PResult SetSocketOption(SocketOption option, BOOL value);

		//IP�o�[�W����
		IPVersion ipversion = IPVersion::IPv4;

		//�\�P�b�g�n���h��
		SocketHandle handle = INVALID_SOCKET;
	};
}
