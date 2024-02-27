//ClientMain.cpp
#include <PNet\\IncludeMe.h> 
#include <iostream> 
#include <limits>

using namespace PNet;

int main()
{
		//�l�b�g���[�N��������
	if (Network::Initialize())
	{
		//�������������������Ƃ�\��
		std::cout << "Winsock API �̏������ɐ������܂����B" << std::endl;

		//IPv6�\�P�b�g���쐬
		Socket socket(IPVersion::IPv6);
			//�\�P�b�g�̍쐬�����݂�
		if (socket.Create() == PResult::P_Success)
		{
			//�\�P�b�g�̍쐬�������������Ƃ�\��
			std::cout << "�\�P�b�g������ɍ쐬����܂����B" << std::endl;

			//���[�U�[��IP�A�h���X����͂�����
			std::string ipAddress{};
			int portNumber = 0;
			std::cout << "�ڑ����IP�A�h���X����͂��Ă�������: ";
			std::getline(std::cin, ipAddress);
			std::cout << "�ڑ���̃|�[�g�ԍ�����͂��Ă�������: ";
			std::cin >> portNumber;
			std::cin.ignore();

				//�\�P�b�g������̃G���h�|�C���g�ɐڑ�����悤��
			if (socket.Connect(IPEndpoint(ipAddress.c_str(), portNumber)) == PResult::P_Success)
			{
				//�ڑ��������������Ƃ�\��
				std::cout << "�T�[�o�[�ɐ���ɐڑ�����܂����B\n";

				while (true)
				{
					std::string clientMessage;
					std::cout << "���b�Z�[�W�����: ";
					//�N���C�A���g�̃��b�Z�[�W�����
					std::getline(std::cin, clientMessage);
					clientMessage += '\n';

					Packet clientPacket(PacketType::PT_ChatMessage);
					//�N���C�A���g�̃��b�Z�[�W���p�P�b�g�ɒǉ�
					clientPacket << clientMessage;

					//�p�P�b�g���\�P�b�g�ɑ��M
					socket.Send(clientPacket);

					Packet serverPacket;
						//�\�P�b�g����p�P�b�g����M
					if (socket.Recv(serverPacket) == PResult::P_Success)
					{
						std::string serverMessage;
						//�p�P�b�g����T�[�o�[�̃��b�Z�[�W�𒊏o
						serverPacket >> serverMessage;
						//�T�[�o�[����̃��b�Z�[�W��\��
						std::cout << "Server Message: " << serverMessage;
					}
						//�p�P�b�g�̎�M�����s�����ꍇ�A���[�v�𔲂���
					else if (socket.Recv(serverPacket) != PResult::P_Success)
					{
						break;

						Sleep(500);
					}

				}
			}
			else
			{
				//�T�[�o�[�ւ̐ڑ������s�������Ƃ�\��
				std::cerr << "�T�[�o�[�ւ̐ڑ��Ɏ��s���܂����B" << std::endl;
			}
			//�\�P�b�g�����
			socket.Close();
		}
		else
		{
			//�\�P�b�g�̍쐬�����s�������Ƃ�\��
			std::cerr << "�\�P�b�g�̍쐬�Ɏ��s���܂����B" << std::endl;
		}
	}
	//�l�b�g���[�N���V���b�g�_�E��
	Network::Shutdown();
	//���[�U�[�������L�[�������܂Ńv���O�������ꎞ��~
	system("pause");
	return 0;
}
