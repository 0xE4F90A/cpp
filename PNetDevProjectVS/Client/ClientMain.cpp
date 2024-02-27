//ClientMain.cpp
#include <PNet\\IncludeMe.h> 
#include <iostream> 
#include <limits>

using namespace PNet;

int main()
{
		//ネットワークを初期化
	if (Network::Initialize())
	{
		//初期化が成功したことを表示
		std::cout << "Winsock API の初期化に成功しました。" << std::endl;

		//IPv6ソケットを作成
		Socket socket(IPVersion::IPv6);
			//ソケットの作成を試みる
		if (socket.Create() == PResult::P_Success)
		{
			//ソケットの作成が成功したことを表示
			std::cout << "ソケットが正常に作成されました。" << std::endl;

			//ユーザーにIPアドレスを入力させる
			std::string ipAddress{};
			int portNumber = 0;
			std::cout << "接続先のIPアドレスを入力してください: ";
			std::getline(std::cin, ipAddress);
			std::cout << "接続先のポート番号を入力してください: ";
			std::cin >> portNumber;
			std::cin.ignore();

				//ソケットが特定のエンドポイントに接続するように
			if (socket.Connect(IPEndpoint(ipAddress.c_str(), portNumber)) == PResult::P_Success)
			{
				//接続が成功したことを表示
				std::cout << "サーバーに正常に接続されました。\n";

				while (true)
				{
					std::string clientMessage;
					std::cout << "メッセージを入力: ";
					//クライアントのメッセージを入力
					std::getline(std::cin, clientMessage);
					clientMessage += '\n';

					Packet clientPacket(PacketType::PT_ChatMessage);
					//クライアントのメッセージをパケットに追加
					clientPacket << clientMessage;

					//パケットをソケットに送信
					socket.Send(clientPacket);

					Packet serverPacket;
						//ソケットからパケットを受信
					if (socket.Recv(serverPacket) == PResult::P_Success)
					{
						std::string serverMessage;
						//パケットからサーバーのメッセージを抽出
						serverPacket >> serverMessage;
						//サーバーからのメッセージを表示
						std::cout << "Server Message: " << serverMessage;
					}
						//パケットの受信が失敗した場合、ループを抜ける
					else if (socket.Recv(serverPacket) != PResult::P_Success)
					{
						break;

						Sleep(500);
					}

				}
			}
			else
			{
				//サーバーへの接続が失敗したことを表示
				std::cerr << "サーバーへの接続に失敗しました。" << std::endl;
			}
			//ソケットを閉じる
			socket.Close();
		}
		else
		{
			//ソケットの作成が失敗したことを表示
			std::cerr << "ソケットの作成に失敗しました。" << std::endl;
		}
	}
	//ネットワークをシャットダウン
	Network::Shutdown();
	//ユーザーが何かキーを押すまでプログラムを一時停止
	system("pause");
	return 0;
}
