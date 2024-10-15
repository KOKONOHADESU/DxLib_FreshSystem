#pragma once

#include <vector>
#include <string>
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
	class CSVReadOnly
	{
	public:
		/// <summary>
		/// CSVファイルが存在するパスを指定
		/// </summary>
		/// <param name="filePath"></param>
		CSVReadOnly(const std::string& filePath)
		{
			m_filePath = filePath;
		};

		~CSVReadOnly()
		{
			
		};
		
		/// <summary>
/// 
/// </summary>


		/// <summary>
		/// CSVファイルの読み込む
		/// </summary>
		/// <param name="fileName"     >ファイルの名前を指定      </param>
		/// <param name="ignoreCellNum">無視するセルの数          </param>
		/// <returns                   >true : 成功 , false : 失敗</returns>
		bool Load(const std::string& fileName , const int ignoreCellNum)
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
				
				m_readData.push_back(strvec);
			}

			// ファイルを閉じる
			ifs.close();

			return true;
		}

		/// <summary>
		/// 読み込んだデータの一部を読み込む
		/// </summary>
		/// <param name="columns">列を指定(│)</param>
		/// <param name="row"    >行を指定(─)</param>
		/// <returns></returns>
		std::string StringData(const int columns , const int row)
		{
			return m_readData[columns][row];
		}

	private:
		/// <summary>
		/// CSV切り抜き用
		/// </summary>
		/// <param name="input"    >CSVデータの文字列</param>
		/// <param name="delimiter">切り抜く際の文字 </param>
		/// <returns></returns>
		std::vector<std::string> Split(const std::string& input, char delimiter)
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
		std::vector<std::vector<std::string>> m_readData;
	};
}

