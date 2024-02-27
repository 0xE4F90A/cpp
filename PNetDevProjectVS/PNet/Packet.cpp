//Packet.cpp
#include "Packet.h" 
#include "Constants.h" 

namespace PNet
{
	//�p�P�b�g�^�C�v�������Ɏ��R���X�g���N�^���`
	Packet::Packet(PacketType packetType)
	{
		//�p�P�b�g���N���A
		Clear();
		//�p�P�b�g�^�C�v�����蓖�Ă܂��B
		AssignPacketType(packetType);
	}

	//�p�P�b�g�^�C�v���擾����֐����`
	PacketType Packet::GetPacketType()
	{
		//�o�b�t�@�̍ŏ��̗v�f��PacketType�ւ̃|�C���^�Ƃ��ĉ���
		PacketType* packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
		//�l�b�g���[�N�o�C�g�I�[�_�[����z�X�g�o�C�g�I�[�_�[�ɕϊ�
		return static_cast<PacketType>(ntohs(*packetTypePtr));
	}

	//�p�P�b�g�^�C�v�����蓖�Ă�֐����`
	void Packet::AssignPacketType(PacketType packetType)
	{
		//�o�b�t�@�̍ŏ��̗v�f��PacketType�ւ̃|�C���^�Ƃ��ĉ���
		PacketType* packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
		//�z�X�g�o�C�g�I�[�_�[����l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ�
		*packetTypePtr = static_cast<PacketType>(htons(packetType));
	}

	//�p�P�b�g���N���A����֐����`
	void Packet::Clear()
	{
		//�o�b�t�@�̃T�C�Y��PacketType�̃T�C�Y�Ƀ��T�C�Y
		buffer.resize(sizeof(PacketType));
		//�p�P�b�g�^�C�v�𖳌��ɐݒ�
		AssignPacketType(PacketType::PT_Invalid);
		//���o�I�t�Z�b�g��PacketType�̃T�C�Y�ɐݒ�
		extractionOffset = sizeof(PacketType);
	}

	//�f�[�^��ǉ�����֐����`
	void Packet::Append(const void* data, uint32_t size)
	{
		//�p�P�b�g�̃T�C�Y���ő�p�P�b�g�T�C�Y�𒴂���ꍇ�A��O���X���[
		if ((buffer.size() + size) > PNet::g_MaxPacketSize)
			throw PacketException("[Packet::Append(const void*, uint32_t)] - Packet size exceeded max packet size.");

		//�o�b�t�@�̖����Ƀf�[�^��ǉ�
		buffer.insert(buffer.end(), (char*)data, (char*)data + size);
	}

	//uint32_t�^�̃f�[�^���p�P�b�g�ɒǉ����鉉�Z�q���`
	Packet& Packet::operator<<(uint32_t data)
	{
		//�f�[�^���z�X�g�o�C�g�I�[�_�[����l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ�
		data = htonl(data);
		//�f�[�^���p�P�b�g�ɒǉ�
		Append(&data, sizeof(uint32_t));
		//���g�̎Q�Ƃ�Ԃ�
		return *this;
	}

	//uint32_t�^�̃f�[�^���p�P�b�g���璊�o���鉉�Z�q���`
	Packet& Packet::operator>>(uint32_t& data)
	{
		//���o�I�t�Z�b�g���o�b�t�@�̃T�C�Y�𒴂���ꍇ�A��O���X���[
		if ((extractionOffset + sizeof(uint32_t)) > buffer.size())
			throw PacketException("[Packet::operator >>(uint32_t &)] - Extraction offset exceeded buffer size.");

		//�o�b�t�@����f�[�^�𒊏o
		data = *reinterpret_cast<uint32_t*>(&buffer[extractionOffset]);
		//�f�[�^���l�b�g���[�N�o�C�g�I�[�_�[����z�X�g�o�C�g�I�[�_�[�ɕϊ�
		data = ntohl(data);
		//���o�I�t�Z�b�g���X�V
		extractionOffset += sizeof(uint32_t);
		//���g�̎Q�Ƃ�Ԃ�
		return *this;
	}
	//std::string�^�̃f�[�^���p�P�b�g�ɒǉ����鉉�Z�q���`
	Packet& Packet::operator<<(const std::string& data)
	{
		//������̃T�C�Y���p�P�b�g�ɒǉ�
		*this << (uint32_t)data.size();
		//������̃f�[�^���p�P�b�g�ɒǉ�
		Append(data.data(), data.size());
		//���g�̎Q�Ƃ�Ԃ�
		return *this;
	}
	//std::string�^�̃f�[�^���p�P�b�g���璊�o���鉉�Z�q���`
	Packet& Packet::operator>>(std::string& data)
	{
		//��������N���A
		data.clear();

		//������̃T�C�Y��0�ɏ�����
		uint32_t stringSize = 0;
		//������̃T�C�Y���p�P�b�g���璊�o
		*this >> stringSize;

		//���o�I�t�Z�b�g���o�b�t�@�̃T�C�Y�𒴂���ꍇ�A��O���X���[
		if ((extractionOffset + stringSize) > buffer.size())
			throw PacketException("[Packet::operator >>(std::string &)] - Extraction offset exceeded buffer size.");

		//������̃T�C�Y�����T�C�Y
		data.resize(stringSize);
		//�o�b�t�@���當����𒊏o
		data.assign(&buffer[extractionOffset], stringSize);
		//���o�I�t�Z�b�g���X�V
		extractionOffset += stringSize;
		//���g�̎Q�Ƃ�Ԃ�
		return *this;
	}
}
