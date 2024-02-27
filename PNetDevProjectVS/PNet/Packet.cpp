//Packet.cpp
#include "Packet.h" 
#include "Constants.h" 

namespace PNet
{
	//パケットタイプを引数に取るコンストラクタを定義
	Packet::Packet(PacketType packetType)
	{
		//パケットをクリア
		Clear();
		//パケットタイプを割り当てます。
		AssignPacketType(packetType);
	}

	//パケットタイプを取得する関数を定義
	PacketType Packet::GetPacketType()
	{
		//バッファの最初の要素をPacketTypeへのポインタとして解釈
		PacketType* packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
		//ネットワークバイトオーダーからホストバイトオーダーに変換
		return static_cast<PacketType>(ntohs(*packetTypePtr));
	}

	//パケットタイプを割り当てる関数を定義
	void Packet::AssignPacketType(PacketType packetType)
	{
		//バッファの最初の要素をPacketTypeへのポインタとして解釈
		PacketType* packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
		//ホストバイトオーダーからネットワークバイトオーダーに変換
		*packetTypePtr = static_cast<PacketType>(htons(packetType));
	}

	//パケットをクリアする関数を定義
	void Packet::Clear()
	{
		//バッファのサイズをPacketTypeのサイズにリサイズ
		buffer.resize(sizeof(PacketType));
		//パケットタイプを無効に設定
		AssignPacketType(PacketType::PT_Invalid);
		//抽出オフセットをPacketTypeのサイズに設定
		extractionOffset = sizeof(PacketType);
	}

	//データを追加する関数を定義
	void Packet::Append(const void* data, uint32_t size)
	{
		//パケットのサイズが最大パケットサイズを超える場合、例外をスロー
		if ((buffer.size() + size) > PNet::g_MaxPacketSize)
			throw PacketException("[Packet::Append(const void*, uint32_t)] - Packet size exceeded max packet size.");

		//バッファの末尾にデータを追加
		buffer.insert(buffer.end(), (char*)data, (char*)data + size);
	}

	//uint32_t型のデータをパケットに追加する演算子を定義
	Packet& Packet::operator<<(uint32_t data)
	{
		//データをホストバイトオーダーからネットワークバイトオーダーに変換
		data = htonl(data);
		//データをパケットに追加
		Append(&data, sizeof(uint32_t));
		//自身の参照を返す
		return *this;
	}

	//uint32_t型のデータをパケットから抽出する演算子を定義
	Packet& Packet::operator>>(uint32_t& data)
	{
		//抽出オフセットがバッファのサイズを超える場合、例外をスロー
		if ((extractionOffset + sizeof(uint32_t)) > buffer.size())
			throw PacketException("[Packet::operator >>(uint32_t &)] - Extraction offset exceeded buffer size.");

		//バッファからデータを抽出
		data = *reinterpret_cast<uint32_t*>(&buffer[extractionOffset]);
		//データをネットワークバイトオーダーからホストバイトオーダーに変換
		data = ntohl(data);
		//抽出オフセットを更新
		extractionOffset += sizeof(uint32_t);
		//自身の参照を返す
		return *this;
	}
	//std::string型のデータをパケットに追加する演算子を定義
	Packet& Packet::operator<<(const std::string& data)
	{
		//文字列のサイズをパケットに追加
		*this << (uint32_t)data.size();
		//文字列のデータをパケットに追加
		Append(data.data(), data.size());
		//自身の参照を返す
		return *this;
	}
	//std::string型のデータをパケットから抽出する演算子を定義
	Packet& Packet::operator>>(std::string& data)
	{
		//文字列をクリア
		data.clear();

		//文字列のサイズを0に初期化
		uint32_t stringSize = 0;
		//文字列のサイズをパケットから抽出
		*this >> stringSize;

		//抽出オフセットがバッファのサイズを超える場合、例外をスロー
		if ((extractionOffset + stringSize) > buffer.size())
			throw PacketException("[Packet::operator >>(std::string &)] - Extraction offset exceeded buffer size.");

		//文字列のサイズをリサイズ
		data.resize(stringSize);
		//バッファから文字列を抽出
		data.assign(&buffer[extractionOffset], stringSize);
		//抽出オフセットを更新
		extractionOffset += stringSize;
		//自身の参照を返す
		return *this;
	}
}
