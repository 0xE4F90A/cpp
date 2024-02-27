//IPEndpoint.h
#pragma once
#include "IPVersion.h"
#include <string>
#include <vector>
#include <WS2tcpip.h>

namespace PNet
{
	class IPEndpoint
	{
	public:
		//コンストラクタ: IPアドレスとポート番号を引数に取る
		IPEndpoint(const char* ip, unsigned short port);

		//コンストラクタ: sockaddr 構造体を引数に取る
		IPEndpoint(sockaddr* addr);

		//IPバージョンを取得するメソッド
		IPVersion GetIPVersion();

		//IPアドレスのバイト表現を取得するメソッド
		std::vector<uint8_t> GetIPBytes();

		//ホスト名を取得するメソッド
		std::string GetHostname();

		//IPアドレスの文字列表現を取得するメソッド
		std::string GetIPString();

		//ポート番号を取得するメソッド
		unsigned short GetPort();

		//IPv4 の sockaddr_in 構造体を取得するメソッド
		sockaddr_in GetSockaddrIPv4();

		//IPv6 の sockaddr_in6 構造体を取得するメソッド
		sockaddr_in6 GetSockaddrIPv6();

		//エンドポイントの詳細を出力するメソッド
		void Print();

	private:
		//IPバージョン
		IPVersion ipversion = IPVersion::Unknown;

		//ホスト名
		std::string hostname = "";

		//IPアドレスの文字列表現
		std::string ip_string = "";

		//IPアドレスのバイト表現
		std::vector<uint8_t> ip_bytes;

		//ポート番号
		unsigned short port = 0;
	};
}
