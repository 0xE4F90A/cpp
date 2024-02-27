//ServerMain.cpp
#include <PNet\\IncludeMe.h>
#include <iostream>


using namespace PNet;

//�p�P�b�g����������֐����`
bool ProcessPacket(Socket& newConnection, Packet& packet)
{
	//�p�P�b�g�^�C�v�Ɋ�Â��ď����𕪊�
	switch (packet.GetPacketType())
	{
		//�`���b�g���b�Z�[�W�̏ꍇ�̏������`
	case PacketType::PT_ChatMessage:
	{
		std::string clientMessage;
		//�p�P�b�g����`���b�g���b�Z�[�W�𒊏o
		packet >> clientMessage;
		//�N���C�A���g����̃��b�Z�[�W��\��
		std::cout << "Client Message: " << clientMessage;

		std::string serverMessage;
		std::cout << "���b�Z�[�W�����: ";
		//�T�[�o�[�̃��b�Z�[�W�����
		std::getline(std::cin, serverMessage);
        serverMessage += '\n';

		Packet serverPacket(PacketType::PT_ChatMessage);
		//�T�[�o�[�̃��b�Z�[�W���p�P�b�g�ɒǉ�
		serverPacket << serverMessage;

		//�p�P�b�g��V�����ڑ��ɑ��M
		newConnection.Send(serverPacket);
		break;
	}
	default:
		//���m�̃p�P�b�g�^�C�v�̏ꍇ�Afalse
		return false;
	}
	//�p�P�b�g�̏��������������ꍇ�Atrue
	return true;
}

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
            std::cout << "�T�[�o�[��IP�A�h���X����͂��Ă�������: ";
            std::getline(std::cin, ipAddress);
            std::cout << "�T�[�o�[�̃|�[�g�ԍ�����͂��Ă�������: ";
            std::cin >> portNumber;
            std::cin.ignore();

            //�\�P�b�g������̃G���h�|�C���g�Ń��b�X������悤��
            if (socket.Listen(IPEndpoint(ipAddress.c_str(), portNumber)) == PResult::P_Success)
            {
                //���b�X���������������Ƃ�\��
                std::cout << "�\�P�b�g�̓|�[�g" << portNumber << "�Ő���Ƀ��b�X�����Ă���\n";
                //�V�����ڑ��p�̃\�P�b�g���쐬
                Socket newConnection;
                //�V�����ڑ����󂯓����
                if (socket.Accept(newConnection) == PResult::P_Success)
                {
                    //�V�����ڑ����󂯓����ꂽ���Ƃ�\��
                    std::cout << "�V�����ڑ����󂯓�����܂����B" << std::endl;

                    //�p�P�b�g���쐬
                    Packet packet;
                    //�������[�v���J�n
                    while (true)
                    {
                        //�V�����ڑ�����p�P�b�g����M
                        PResult result = newConnection.Recv(packet);
                        //��M�����s�����ꍇ�A���[�v�𔲂���
                        if (result != PResult::P_Success)
                            break;

                        //�p�P�b�g�̏��������s�����ꍇ�A���[�v�𔲂���
                        if (!ProcessPacket(newConnection, packet))
                            break;
                    }

                    //�V�����ڑ������
                    newConnection.Close();
                }
                else
                {
                    //�V�����ڑ��̎󂯓��ꂪ���s�������Ƃ�\��
                    std::cerr << "�V�����ڑ��̎󂯓���Ɏ��s���܂����B" << std::endl;
                }
            }
            else
            {
                //���b�X�������s�������Ƃ�\��
                std::cerr << "�|�[�g" << portNumber << "�ł̃��b�X���Ɏ��s���܂����B" << std::endl;
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
