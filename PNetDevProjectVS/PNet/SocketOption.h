//SocketOption.h
#pragma once
namespace PNet
{
	enum SocketOption
	{
		TCP_NoDelay, //TRUE = nagle �̃A���S���Y���𖳌��ɂ���
		IPV6_Only, //TRUE = IPv6 �݂̂��ڑ��ł��܂��B FALSE = ipv4 �� ipv6 �̗����ɐڑ��ł��܂��B
	};
}