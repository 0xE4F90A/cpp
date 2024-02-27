//Packet.h
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <vector>
#include <winsock.h>
#include "PacketException.h"
#include "PacketType.h"

namespace PNet
{
	class Packet
	{
	public:
		//コンストラクタ: パケットタイプを引数に取る
		Packet(PacketType packetType = PacketType::PT_Invalid);

		//パケットタイプを取得するメソッド
		PacketType GetPacketType();

		//パケットタイプを設定するメソッド
		void AssignPacketType(PacketType packetType);


		//パケットをクリアするメソッド
		void Clear();

		//パケットにデータを追加するメソッド
		void Append(const void* data, uint32_t size);


		//uint32_t 型のデータをパケットに追加する演算子オーバーロード
		Packet& operator << (uint32_t data);

		//uint32_t 型のデータをパケットから抽出する演算子オーバーロード
		Packet& operator >> (uint32_t& data);


		//std::string 型のデータをパケットに追加する演算子オーバーロード
		Packet& operator << (const std::string& data);

		//std::string 型のデータをパケットから抽出する演算子オーバーロード
		Packet& operator >> (std::string& data);

		//データ抽出のオフセット
		uint32_t extractionOffset = 0;

		//パケットデータを格納するバッファ
		std::vector<char> buffer;
	};
}
