//Socket.cpp

#include "Socket.h"
#include <assert.h>
#include <iostream>

namespace PNet
{
	//Socketクラスのコンストラクタ
	Socket::Socket(IPVersion ipversion, SocketHandle handle)
		:ipversion(ipversion), handle(handle)
	{
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6); //IPバージョンがIPv4またはIPv6であることを確認
	}

	//ソケットを作成する関数
	PResult Socket::Create()
	{
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6); //IPバージョンがIPv4またはIPv6であることを確認

		if (handle != INVALID_SOCKET)
		{
			return PResult::P_GenericError; //エラー: 既に有効なソケットが存在する場合
		}

		handle = socket((ipversion == IPVersion::IPv4) ? AF_INET : AF_INET6, SOCK_STREAM, IPPROTO_TCP); //ソケットを作成

		if (handle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError; //エラー: ソケットの作成に失敗
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != PResult::P_Success)
		{
			return PResult::P_GenericError; //エラー: TCP_NoDelayオプションの設定に失敗
		}

		return PResult::P_Success; //成功
	}

	//ソケットを閉じる関数
	PResult Socket::Close()
	{
		if (handle == INVALID_SOCKET)
		{
			return PResult::P_GenericError; //エラー: 無効なソケットを閉じようとしている
		}

		int result = closesocket(handle);
		if (result != 0) //エラー: ソケットのクローズに失敗
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		handle = INVALID_SOCKET;
		return PResult::P_Success; //成功
	}

	//ソケットにアドレスをバインドする関数
	PResult Socket::Bind(IPEndpoint endpoint)
	{
		assert(ipversion == endpoint.GetIPVersion()); //IPバージョンが一致することを確認

		if (ipversion == IPVersion::IPv4)
		{
			sockaddr_in addr = endpoint.GetSockaddrIPv4();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
			if (result != 0) //エラー: バインドに失敗
			{
				int error = WSAGetLastError();
				return PResult::P_GenericError;
			}
		}
		else //IPv6
		{
			sockaddr_in6 addr = endpoint.GetSockaddrIPv6();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in6));
			if (result != 0) //エラー: バインドに失敗
			{
				int error = WSAGetLastError();
				return PResult::P_GenericError;
			}
		}

		return PResult::P_Success; //成功
	}

	//接続待ち状態にする関数
	PResult Socket::Listen(IPEndpoint endpoint, int backlog)
	{
		if (ipversion == IPVersion::IPv6)
		{
			if (SetSocketOption(SocketOption::IPV6_Only, FALSE) != PResult::P_Success)
			{
				return PResult::P_GenericError; //エラー: IPV6_Onlyオプションの設定に失敗
			}
		}


		if (Bind(endpoint) != PResult::P_Success)
		{
			return PResult::P_GenericError; //エラー: バインドに失敗
		}

		int result = listen(handle, backlog);
		if (result != 0) //エラー: リスニングに失敗
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		return PResult::P_Success; //成功
	}

	//接続を受け入れる関数
	PResult Socket::Accept(Socket& outSocket)
	{
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6); //IPバージョンがIPv4またはIPv6であることを確認

		if (ipversion == IPVersion::IPv4)
		{
			sockaddr_in addr = {};
			int len = sizeof(sockaddr_in);
			SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
			if (acceptedConnectionHandle == INVALID_SOCKET)
			{
				int error = WSAGetLastError();
				return PResult::P_GenericError; //エラー: 接続の受け入れに失敗
			}
			IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
			std::cout << "新しい接続を待機しています..." << std::endl;
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
				return PResult::P_GenericError; //エラー: 接続の受け入れに失敗
			}
			IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
			std::cout << "新しい接続を待機しています..." << std::endl;
			newConnectionEndpoint.Print();
			outSocket = Socket(IPVersion::IPv6, acceptedConnectionHandle);
		}

		return PResult::P_Success; //成功
	}

	//接続を確立する関数
	PResult Socket::Connect(IPEndpoint endpoint)
	{
		assert(ipversion == endpoint.GetIPVersion()); //IPバージョンが一致することを確認

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
		if (result != 0) //エラー: 接続に失敗
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}
		return PResult::P_Success; //成功
	}

	//ここの、
	//====================//
	//データを送信する関数//
	//====================//

	PResult Socket::Send(const void* data, int numberOfBytes, int& bytesSent)
	{
		bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);

		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError; //エラー: 送信に失敗
		}

		return PResult::P_Success; //成功
	}

	//データを受信する関数
	PResult Socket::Recv(void* destination, int numberOfBytes, int& bytesReceived)
	{
		bytesReceived = recv(handle, (char*)destination, numberOfBytes, NULL);

		if (bytesReceived == 0) //接続が正常にクローズされた場合
		{
			return PResult::P_GenericError; //エラー: 受信したデータがない
		}

		if (bytesReceived == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError; //エラー: 受信に失敗
		}

		return PResult::P_Success; //成功
	}

	//データをすべて送信する関数
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
				return PResult::P_GenericError; //エラー: 送信に失敗
			}
			totalBytesSent += bytesSent;
		}

		return PResult::P_Success; //成功
	}

	//データをすべて受信する関数
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
				return PResult::P_GenericError; //エラー: 受信に失敗
			}
			totalBytesReceived += bytesReceived;
		}

		return PResult::P_Success; //成功
	}

	//パケットを送信する関数
	PResult Socket::Send(Packet& packet)
	{
		uint16_t encodedPacketSize = htons(packet.buffer.size());
		PResult result = SendAll(&encodedPacketSize, sizeof(uint16_t));
		if (result != PResult::P_Success)
			return PResult::P_GenericError;

		result = SendAll(packet.buffer.data(), packet.buffer.size());
		if (result != PResult::P_Success)
			return PResult::P_GenericError;

		return PResult::P_Success; //成功
	}

	//パケットを受信する関数
	PResult Socket::Recv(Packet& packet)
	{
		packet.Clear();

		uint16_t encodedSize = 0;
		PResult result = RecvAll(&encodedSize, sizeof(uint16_t));
		if (result != PResult::P_Success)
			return PResult::P_GenericError;

		uint16_t bufferSize = ntohs(encodedSize);

		if (bufferSize > PNet::g_MaxPacketSize)
			return PResult::P_GenericError; //エラー: 受信したパケットサイズが大きすぎる

		packet.buffer.resize(bufferSize);
		result = RecvAll(&packet.buffer[0], bufferSize);
		if (result != PResult::P_Success)
			return PResult::P_GenericError; //エラー: 受信に失敗

		return PResult::P_Success; //成功
	}


	//ソケットのハンドルを取得する関数
	SocketHandle Socket::GetHandle()
	{
		return handle;
	}

	//ソケットのIPバージョンを取得する関数
	IPVersion Socket::GetIPVersion()
	{
		return ipversion;
	}

	//ソケットオプションを設定する関数
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
			return PResult::P_GenericError; //エラー: 無効なオプション
		}

		if (result != 0) //エラー: ソケットオプションの設定に失敗
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		return PResult::P_Success; //成功
	}
}
