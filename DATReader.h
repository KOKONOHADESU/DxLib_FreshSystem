#pragma once

#include <string>
#include <map>

/// <summary>
/// .datファイル関連
/// </summary>
namespace Dat
{	
	/// <summary>
	/// .datファイルを作成,上書き保存
	/// </summary>
	/// <typeparam name="T">ID        </typeparam>
	/// <typeparam name="U">データの型</typeparam>
	template<typename T, typename U>
	class DATReader
	{
	public:
		DATReader() {};
		~DATReader() {};

		/// <summary>
		/// .datファイルがある階層のフォルダーを指定
		/// </summary>
		/// <param name="writerFolderPath">書き込み用フォルダのパス指定</param>
		void CSVFolderPath(const char* writerFolderPath)
		{
			m_folderPath = writerFolderPath;
		}

		/// <summary>
		/// 書き込み
		/// </summary>
		/// <param name="fileName" >書き込むファイルネーム</param>
		/// <param name="T"        >ID                    </param>
		/// <param name="U"        >書き込むデータ        </param>
		U Reader(const std::string& fileName, const T& id)
		{						 
			std::ifstream ifs;
			ifs.open(fileName.c_str(), std::ios_base::binary);
			// データをファイルに書き込む
			ifs.read((char*)&m_datData[id], sizeof(U));
			ifs.close();

			return m_datData[id];
		}

		



	private:

		// フォルダーパス
		std::string m_folderPath;

		// .datファイルの内部データ
		std::map<T, U> m_datData;

	};
}
