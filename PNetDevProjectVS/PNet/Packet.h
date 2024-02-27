//Packet.h
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <vector>
#include <winsock.h>
#include "PacketException.h"
#include "PacketType.h"

namespace PNet
{
	class Packet
	{
	public:
		//�R���X�g���N�^: �p�P�b�g�^�C�v�������Ɏ��
		Packet(PacketType packetType = PacketType::PT_Invalid);

		//�p�P�b�g�^�C�v���擾���郁�\�b�h
		PacketType GetPacketType();

		//�p�P�b�g�^�C�v��ݒ肷�郁�\�b�h
		void AssignPacketType(PacketType packetType);


		//�p�P�b�g���N���A���郁�\�b�h
		void Clear();

		//�p�P�b�g�Ƀf�[�^��ǉ����郁�\�b�h
		void Append(const void* data, uint32_t size);


		//uint32_t �^�̃f�[�^���p�P�b�g�ɒǉ����鉉�Z�q�I�[�o�[���[�h
		Packet& operator << (uint32_t data);

		//uint32_t �^�̃f�[�^���p�P�b�g���璊�o���鉉�Z�q�I�[�o�[���[�h
		Packet& operator >> (uint32_t& data);


		//std::string �^�̃f�[�^���p�P�b�g�ɒǉ����鉉�Z�q�I�[�o�[���[�h
		Packet& operator << (const std::string& data);

		//std::string �^�̃f�[�^���p�P�b�g���璊�o���鉉�Z�q�I�[�o�[���[�h
		Packet& operator >> (std::string& data);

		//�f�[�^���o�̃I�t�Z�b�g
		uint32_t extractionOffset = 0;

		//�p�P�b�g�f�[�^���i�[����o�b�t�@
		std::vector<char> buffer;
	};
}
