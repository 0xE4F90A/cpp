//PacketException.h
#pragma once

#include <string>

namespace PNet
{
	class PacketException
	{
	public:
		//コンストラクタを定義例外メッセージを引数に
		PacketException(std::string exception)
			//初期化リストを使用して、メンバ変数exceptionを初期化
			:exception(exception)
		{}
		// これは例外メッセージを返す
		const char* what()
		{
			//例外メッセージをCスタイルの文字列として返す
			return exception.c_str();
		}
		//例外メッセージをstd::stringとして返す
		std::string ToString()
		{
			//例外メッセージを返す
			return exception;
		}
	private:
		//例外メッセージを保持するためのプライベートメンバ変数を定義
		std::string exception;
	};
}
