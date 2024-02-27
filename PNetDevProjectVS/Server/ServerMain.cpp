//ServerMain.cpp
#include <PNet\\IncludeMe.h>
#include <iostream>


using namespace PNet;

//パケットを処理する関数を定義
bool ProcessPacket(Socket& newConnection, Packet& packet)
{
	//パケットタイプに基づいて処理を分岐
	switch (packet.GetPacketType())
	{
		//チャットメッセージの場合の処理を定義
	case PacketType::PT_ChatMessage:
	{
		std::string clientMessage;
		//パケットからチャットメッセージを抽出
		packet >> clientMessage;
		//クライアントからのメッセージを表示
		std::cout << "Client Message: " << clientMessage;

		std::string serverMessage;
		std::cout << "メッセージを入力: ";
		//サーバーのメッセージを入力
		std::getline(std::cin, serverMessage);
        serverMessage += '\n';

		Packet serverPacket(PacketType::PT_ChatMessage);
		//サーバーのメッセージをパケットに追加
		serverPacket << serverMessage;

		//パケットを新しい接続に送信
		newConnection.Send(serverPacket);
		break;
	}
	default:
		//未知のパケットタイプの場合、false
		return false;
	}
	//パケットの処理が成功した場合、true
	return true;
}

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
            std::cout << "サーバーのIPアドレスを入力してください: ";
            std::getline(std::cin, ipAddress);
            std::cout << "サーバーのポート番号を入力してください: ";
            std::cin >> portNumber;
            std::cin.ignore();

            //ソケットが特定のエンドポイントでリッスンするように
            if (socket.Listen(IPEndpoint(ipAddress.c_str(), portNumber)) == PResult::P_Success)
            {
                //リッスンが成功したことを表示
                std::cout << "ソケットはポート" << portNumber << "で正常にリッスンしている\n";
                //新しい接続用のソケットを作成
                Socket newConnection;
                //新しい接続を受け入れる
                if (socket.Accept(newConnection) == PResult::P_Success)
                {
                    //新しい接続が受け入れられたことを表示
                    std::cout << "新しい接続が受け入れられました。" << std::endl;

                    //パケットを作成
                    Packet packet;
                    //無限ループを開始
                    while (true)
                    {
                        //新しい接続からパケットを受信
                        PResult result = newConnection.Recv(packet);
                        //受信が失敗した場合、ループを抜ける
                        if (result != PResult::P_Success)
                            break;

                        //パケットの処理が失敗した場合、ループを抜ける
                        if (!ProcessPacket(newConnection, packet))
                            break;
                    }

                    //新しい接続を閉じる
                    newConnection.Close();
                }
                else
                {
                    //新しい接続の受け入れが失敗したことを表示
                    std::cerr << "新しい接続の受け入れに失敗しました。" << std::endl;
                }
            }
            else
            {
                //リッスンが失敗したことを表示
                std::cerr << "ポート" << portNumber << "でのリッスンに失敗しました。" << std::endl;
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
