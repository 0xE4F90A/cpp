//SocketOption.h
#pragma once
namespace PNet
{
	enum SocketOption
	{
		TCP_NoDelay, //TRUE = nagle のアルゴリズムを無効にする
		IPV6_Only, //TRUE = IPv6 のみが接続できます。 FALSE = ipv4 と ipv6 の両方に接続できます。
	};
}