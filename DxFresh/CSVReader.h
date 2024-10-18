#pragma once

#include "SingletonBase.h"
#include "CSVCommon.h"

#include <vector>
#include <map>
#include <sstream>
#include <fstream>

/// <summary>
/// CSV関連
/// </summary>
namespace CSV
{	
	/// <summary>
	/// CSVファイル読み込み用
	/// </summary>
	/// <typeparam name="T">ID用の型を指定</typeparam>
	/// <typeparam name="U">どのシーンで画像を読み込むかをしているする型</typeparam>
	template <typename T, typename U>
	class CSVReader : public SingletonBase<CSVReader<T, U>>
	{
	private:
		/// <summary>
		/// 読み込み用データ
		/// </summary>
		struct readerData
		{
			T id;                                             // ID
			U scene;                                          // シーン	
			std::string filePath;                             // パス
			std::vector<std::vector<std::string>> stringData; // CSVから読み込んだ文字
			int ignoreCellNum;                                // 無視するセル
			bool isNoEnd;                                     // どのシーンでも使用する場合
		};
	public:
		// SingletonBaseクラスのアクセスを許可する
		friend class SingletonBase<CSVReader<T, U>>;

	private:
		CSVReader(){};

	public:
		~CSVReader(){};

		/// <summary>
		/// 画像がある階層のフォルダーを指定
		/// </summary>
		/// <param name="grahicFolderPath"></param>
		void CSVFolderPath(const char* grahicFolderPath)
		{
			m_folderPath = grahicFolderPath;
		}

		/// <summary>
		/// CSVファイルの読み込む
		/// </summary>
		/// <param name="id"           >IDを指定する                </param>
		/// <param name="scene"        >使用するシーンの型          </param>
		/// <param name="fileName"     >ファイルの名前を指定        </param>
		/// <param name="ignoreCellNum">無視するセルの数            </param>
		/// <param name="isNoEnd"      >他のシーンで使用するかどうか</param>
		void Add(const T& id, const U& scene, const char* fileName, const int ignoreCellNum, bool isNoEnd = false)
		{
			readerData data{};

			// ID
			data.id = id;

			// 使用するシーン
			data.scene = scene;

			// ファイルパス
			data.filePath = m_folderPath + fileName + kExtension;

			// すべてのシーンで使用するかどうか
			if (isNoEnd)
			{				
				// 読み込み
				data.stringData = Read(data.filePath.c_str(), ignoreCellNum);
			}
			else
			{
				// 読み込まない
				data.stringData = { {""} };
			}

			// 無視するセル
			data.ignoreCellNum = ignoreCellNum;

			// すべてのシーンで使用するかどうか
			data.isNoEnd = isNoEnd;			

			// データの追加
			m_readData.push_back(data);
		}

		/// <summary>
		/// 現在のシーンを確認する
		/// </summary>
		/// <param name="scene">現在のシーン</param>
		void SceneInput(const U& scene)
		{
			// すべてのデータを確認
			for (int i = 0; i < m_readData.size(); i++)
			{				
				// 現在のシーンで使用するかどうか
				if (m_readData[i].scene == scene)
				{
					// ロード
					m_readData[i].stringData = Read(m_readData[i].filePath.c_str(), m_readData[i].ignoreCellNum);

					continue;
				}
				// 他シーンで使用する場合
				else if (m_readData[i].scene != scene && !m_readData[i].isNoEnd)
				{									
					m_readData[i].stringData = { {""} };

					continue;				
				}
			}
		}

		/// <summary>
		/// CSVファイルを読み込んで渡す
		/// </summary>
		/// <param name="fileName"     >ファイル名             </param>
		/// <param name="ignoreCellNum">無視するセル           </param>
		/// <returns                   >CSVから読み込んだ文字列</returns>
		std::vector<std::vector<std::string>> Read(const char* fileName, int ignoreCellNum)
		{
			// ファイルから1文字ずつ読み込む用
			std::string line;

			// ファイル読み込み用
			std::ifstream ifs(fileName);

			// ファイルが開けない場合
			if (!ifs)
			{
				
			}

			// 無視するセルをカウントする
			int ignoreCellCount = 0;

			std::vector<std::vector<std::string>> string;

			// 読み込み開始
			while (getline(ifs, line))
			{
				// 読み込まないセルを飛ばす
				ignoreCellCount++;
				if (ignoreCellCount == ignoreCellNum) continue;

				// 読み込んだ行をカンマで分割する
				std::vector<std::string> strvec = Split(line, ',');

				// データを記録
				string.push_back(strvec);
			}

			// ファイルを閉じる
			ifs.close();

			return string;
		}
		
		/// <summary>
		/// 読み込んだデータの一部を読み込む
		/// </summary>
		/// <typeparam name="U"       >IDを指定    <</typeparam>
		/// <param name="loadDataType">指定した型  </param>
		/// <param name="columns"     >列を指定(│)</param>
		/// <param name="row"         >行を指定(─)</param>
		/// <returns                  >指定したセルの要素を指定した型で返す/returns>
		template <typename V>
		V StringData(const T& id, const int columns, const int row)
		{
			std::stringstream cellStream{};

			// すべての確認
			for (int i = 0; i < m_readData.size(); i++)
			{
				// 使用するシーンを確認
				if (m_readData[i].id == id)
				{
					// 指定のセルから文字列を確認
					std::string data = m_readData[i].stringData[row][columns];
					// ストリームにデータを挿入
					cellStream << data; 

					break;
				}
			}

			// 指定した型の変数を作成
			V value{};

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
		// フォルダパス記録
		std::string m_folderPath;

		// 読み込んだデータ
		std::vector<readerData> m_readData;
	};
}

