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
		//コンストラクタ
		Socket( IPVersion ipversion = IPVersion::IPv4,
				SocketHandle handle = INVALID_SOCKET);

		//ソケットの作成
		PResult Create();

		//ソケットのクローズ
		PResult Close();

		//エンドポイントへのバインド
		PResult Bind(IPEndpoint endpoint);

		//エンドポイントでのリッスン
		PResult Listen(IPEndpoint endpoint, int backlog = 5);

		//ソケットの受け入れ
		PResult Accept(Socket& outSocket);

		//エンドポイントへの接続
		PResult Connect(IPEndpoint endpoint);

		//データの送信
		PResult Send(const void* data, int numberOfBytes, int& bytesSent);

		//データの受信
		PResult Recv(void* destination, int numberOfBytes, int& bytesReceived);

		//全データの送信
		PResult SendAll(const void* data, int numberOfBytes);

		//全データの受信
		PResult RecvAll(void* destination, int numberOfBytes);

		//パケットの送信
		PResult Send(Packet& packet);

		//パケットの受信
		PResult Recv(Packet& packet);

		//ソケットハンドルの取得
		SocketHandle GetHandle();

		//IPバージョンの取得
		IPVersion GetIPVersion();

	private:
		//ソケットオプションの設定
		PResult SetSocketOption(SocketOption option, BOOL value);

		//IPバージョン
		IPVersion ipversion = IPVersion::IPv4;

		//ソケットハンドル
		SocketHandle handle = INVALID_SOCKET;
	};
}
