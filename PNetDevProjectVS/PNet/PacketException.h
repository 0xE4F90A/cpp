//PacketException.h
#pragma once

#include <string>

namespace PNet
{
	class PacketException
	{
	public:
		//�R���X�g���N�^���`��O���b�Z�[�W��������
		PacketException(std::string exception)
			//���������X�g���g�p���āA�����o�ϐ�exception��������
			:exception(exception)
		{}
		// ����͗�O���b�Z�[�W��Ԃ�
		const char* what()
		{
			//��O���b�Z�[�W��C�X�^�C���̕�����Ƃ��ĕԂ�
			return exception.c_str();
		}
		//��O���b�Z�[�W��std::string�Ƃ��ĕԂ�
		std::string ToString()
		{
			//��O���b�Z�[�W��Ԃ�
			return exception;
		}
	private:
		//��O���b�Z�[�W��ێ����邽�߂̃v���C�x�[�g�����o�ϐ����`
		std::string exception;
	};
}
