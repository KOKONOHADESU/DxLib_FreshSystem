#pragma once

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

#include <DxLib.h>

namespace
{
	// 拡張子
	const std::string kExtension = ".csv";
}

/// <summary>
/// CSV関連
/// </summary>
namespace CSV
{	
	/// <summary>
	/// CSVファイル読み込み用
	/// </summary>
	/// <typeparam name="T">ID用の型を指定</typeparam>
	template <class T>
	class CSVReader final
	{
	public:
		/// <summary>
		/// CSVファイルが存在するパスを指定
		/// </summary>
		/// <param name="filePath"></param>		
		CSVReader(const std::string& filePath)
		{
			m_filePath = filePath;
		};

		~CSVReader()
		{
			
		};

		/// <summary>
		/// CSVファイルの読み込む
		/// </summary>
		/// <param name="id"           >IDを指定する              </param>
		/// <param name="fileName"     >ファイルの名前を指定      </param>
		/// <param name="ignoreCellNum">無視するセルの数          </param>
		/// <returns                   >true : 成功 , false : 失敗</returns>
		bool Load(const T& id, const std::string& fileName, const int ignoreCellNum)
		{
			// ファイルから1文字ずつ読み込む用
			std::string line;

			std::string	filePath = m_filePath + fileName + kExtension;

			// ファイル読み込み用
			std::ifstream ifs(filePath.c_str());

			// ファイルが開けない場合
			if (!ifs)
			{
				return false;
			}

			// 無視するセルをカウントする
			int ignoreCellCount = 0;

			// 読み込み開始
			while (getline(ifs, line))
			{
				// 読み込まないセルを飛ばす
				ignoreCellCount++;
				if (ignoreCellCount == ignoreCellNum) continue;

				// 読み込んだ行をカンマで分割する
				std::vector<std::string> strvec = Split(line, ',');
				
				m_readData[id].push_back(strvec);
			}

			// ファイルを閉じる
			ifs.close();

			return true;
		}
		
		/// <summary>
		/// 読み込んだデータの一部を読み込む
		/// </summary>
		/// <typeparam name="U"       >IDを指定    <</typeparam>
		/// <param name="loadDataType">指定した型  </param>
		/// <param name="columns"     >列を指定(│)</param>
		/// <param name="row"         >行を指定(─)</param>
		/// <returns                  >指定したセルの要素を指定した型で返す/returns>
		template <typename U>
		U StringData(const T& id, const int columns, const int row)
		{
			// 指定した文字列を取得
			std::stringstream cellStream(m_readData[id][columns][row]);

			// 指定した型の変数を作成
			U value{};

			// ストリーム演算子を使い指定した型に変換する
			cellStream >> value;

			// 値を返す
			return value;
		}

	private:
		/// <summary>
		/// CSV切り抜き用
		/// </summary>
		/// <param name="input"    >CSVデータの文字列</param>
		/// <param name="delimiter">切り抜く際の文字 </param>
		/// <returns></returns>
		std::vector<std::string> Split(const std::string& input, char delimiter) const
		{
			// 入力文字列をストリームに変換
			std::istringstream stream(input);

			// 分割された各フィールドを格納するための変数
			std::string field;

			// 分割された文字列を格納するベクター
			std::vector<std::string> result;

			// ストリームから1つずつフィールドを取り出し、ベクターに追加
			while (getline(stream, field, delimiter))
			{
				result.push_back(field);
			}

			// 分割された文字列のベクターを返す
			return result;
		}

	private:
		// ファイルパス
		std::string m_filePath;

		// 読み込んだデータ
		std::map<T,std::vector<std::vector<std::string>>> m_readData;
	};
}

