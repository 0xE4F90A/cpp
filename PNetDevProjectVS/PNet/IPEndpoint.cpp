//IPEndpoint.cpp
#include "IPEndpoint.h"
#include <assert.h>
#include <iostream>

namespace PNet
{
	//IPアドレスとポート番号を指定してIPEndpointオブジェクトを作成するコンストラクタ
	IPEndpoint::IPEndpoint(const char* ip, unsigned short port)
	{
		this->port = port;

		//IPv4の場合
		in_addr addr{}; //IPv4アドレスを格納する場所
		int result = inet_pton(AF_INET, ip, &addr);

		if (result == 1)
		{
			if (addr.S_un.S_addr != INADDR_NONE)
			{
				ip_string = ip;
				hostname = ip;

				ip_bytes.resize(sizeof(ULONG));
				memcpy(&ip_bytes[0], &addr.S_un.S_addr, sizeof(ULONG));

				ipversion = IPVersion::IPv4;
				return;
			}
		}

		//ホスト名をIPv4アドレスに解決しようとする
		addrinfo hints = {}; //getaddrinfoの結果をフィルタリングするためのヒント
		hints.ai_family = AF_INET; //IPv4アドレスのみ
		addrinfo* hostinfo = nullptr;
		result = getaddrinfo(ip, NULL, &hints, &hostinfo);
		if (result == 0)
		{
			sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);

			ip_string.resize(16);
			inet_ntop(AF_INET, &host_addr->sin_addr, &ip_string[0], 16);

			hostname = ip;

			ULONG ip_long = host_addr->sin_addr.S_un.S_addr; //unsigned longとしてIPアドレスを取得
			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &ip_long, sizeof(ULONG)); //IPアドレスを表すバイト配列にコピー

			ipversion = IPVersion::IPv4;

			freeaddrinfo(hostinfo); //getaddrinfoの呼び出しからのメモリ解放
			return;
		}

		//IPv6の場合
		in6_addr addr6{}; //IPv6アドレスを格納する場所
		result = inet_pton(AF_INET6, ip, &addr6);

		if (result == 1)
		{
			ip_string = ip;
			hostname = ip;

			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &addr6.u, 16);

			ipversion = IPVersion::IPv6;
			return;
		}

		//ホスト名をIPv6アドレスに解決しようとする
		addrinfo hintsv6 = {}; //getaddrinfoの結果をフィルタリングするためのヒント
		hintsv6.ai_family = AF_INET6; //IPv6アドレスのみ
		addrinfo* hostinfov6 = nullptr;
		result = getaddrinfo(ip, NULL, &hintsv6, &hostinfov6);
		if (result == 0)
		{
			sockaddr_in6* host_addr = reinterpret_cast<sockaddr_in6*>(hostinfov6->ai_addr);

			ip_string.resize(46);
			inet_ntop(AF_INET6, &host_addr->sin6_addr, &ip_string[0], 46);

			hostname = ip;

			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &host_addr->sin6_addr, 16); //IPアドレスを表すバイト配列にコピー

			ipversion = IPVersion::IPv6;

			freeaddrinfo(hostinfov6); //getaddrinfoの呼び出しからのメモリ解放
			return;
		}
	}

	//sockaddr構造体からIPEndpointオブジェクトを作成するコンストラクタ
	IPEndpoint::IPEndpoint(sockaddr* addr)
	{
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
		if (addr->sa_family == AF_INET) //IPv4の場合
		{
			sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
			ipversion = IPVersion::IPv4;
			port = ntohs(addrv4->sin_port);
			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &addrv4->sin_addr, sizeof(ULONG));
			ip_string.resize(16);
			inet_ntop(AF_INET, &addrv4->sin_addr, &ip_string[0], 16);
			hostname = ip_string;
		}
		else //IPv6の場合
		{
			sockaddr_in6* addrv6 = reinterpret_cast<sockaddr_in6*>(addr);
			ipversion = IPVersion::IPv6;
			port = ntohs(addrv6->sin6_port);
			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &addrv6->sin6_addr, 16);
			ip_string.resize(46);
			inet_ntop(AF_INET6, &addrv6->sin6_addr, &ip_string[0], 46);
			hostname = ip_string;
		}
	}

	//IPのバージョンを取得するメソッド
	IPVersion IPEndpoint::GetIPVersion()
	{
		return ipversion;
	}

	//IPアドレスのバイト表現を取得するメソッド
	std::vector<uint8_t> IPEndpoint::GetIPBytes()
	{
		return ip_bytes;
	}

	//ホスト名を取得するメソッド
	std::string IPEndpoint::GetHostname()
	{
		return hostname;
	}

	//IPアドレスを文字列で取得するメソッド
	std::string IPEndpoint::GetIPString()
	{
		return ip_string;
	}

	//ポート番号を取得するメソッド
	unsigned short IPEndpoint::GetPort()
	{
		return port;
	}

	//IPv4アドレスを表すsockaddr_in構造体を取得するメソッド
	sockaddr_in IPEndpoint::GetSockaddrIPv4()
	{
		assert(ipversion == IPVersion::IPv4);
		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr, &ip_bytes[0], sizeof(ULONG));
		addr.sin_port = htons(port);
		return addr;
	}

	//IPv6アドレスを表すsockaddr_in6構造体を取得するメソッド
	sockaddr_in6 IPEndpoint::GetSockaddrIPv6()
	{
		assert(ipversion == IPVersion::IPv6);
		sockaddr_in6 addr = {};
		addr.sin6_family = AF_INET6;
		memcpy(&addr.sin6_addr, &ip_bytes[0], 16);
		addr.sin6_port = htons(port);
		return addr;
	}

	//IPEndpointオブジェクトの情報をコンソールに出力するメソッド
	void IPEndpoint::Print()
	{
		switch (ipversion)
		{
		case IPVersion::IPv4:
			std::cout << "IP Version: IPv4" << std::endl;
			break;
		case IPVersion::IPv6:
			std::cout << "IP Version: IPv6" << std::endl;
			break;
		default:
			std::cout << "IP Version: Unknown" << std::endl;
		}
		std::cout << "Hostname: " << hostname << std::endl;
		std::cout << "IP: " << ip_string << std::endl;
		std::cout << "Port: " << port << std::endl;
		std::cout << "IP bytes: ";
		for (auto& digit : ip_bytes)
		{
			std::cout << (int)digit;
		}
		std::cout << std::endl;
	}
}
