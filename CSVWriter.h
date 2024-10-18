#pragma once

#include "SingletonBase.h"
#include "CSVCommon.h"

#include <vector>
#include <sstream>
#include <fstream>

/// <summary>
/// CSV関連
/// </summary>
namespace CSV
{
	/// <summary>
	/// CSVファイル書き込み用
	/// </summary>
	class CSVWriter : public SingletonBase<CSVWriter>
	{
	public:
		/// <summary>
		/// SingletonBaseクラスのアクセスを許可する 
		/// </summary>
		friend class SingletonBase<CSVWriter>;

	private:
		CSVWriter() {};

	public:
		~CSVWriter() {};		

		/// <summary>
		/// 画像がある階層のフォルダーを指定
		/// </summary>
		/// <param name="writerFolderPath">書き込み用フォルダのパス指定</param>
		void CSVFolderPath(const char* writerFolderPath)
		{
			m_filePath = writerFolderPath;
		}

		/// <summary>
		/// 書き込み
		/// </summary>
		/// <param name="fileName" >書き込むファイルネーム</param>
		/// <param name="wirteData">書き込む内容          </param>
		void Write(const std::string& fileName, const std::vector<std::vector<std::string>>& wirteData)
		{
			std::ofstream ofs(m_filePath + fileName + kExtension);

			// ファイルが正しく開かれたか確認
			if (!ofs.is_open())
			{
				return;
			}

			// データをCSV形式でファイルに書き込む
			for (const auto& row : wirteData)
			{
				for (int i = 0; i < static_cast<int>(row.size()); ++i)
				{
					ofs << row[i];

					if (i != row.size() - 1)
					{
						// カンマでセルを区切る
						ofs << ",";
					}
				}

				// 改行で行を区切る
				ofs << "\n";
			}

			// ファイルを閉じる
			ofs.close();
		}

	private:

		// ファイルパス記録
		std::string m_filePath;
	};
}

