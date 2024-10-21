#pragma once

#include <string>
#include <map>

#include "DATCommon.h"

/// <summary>
/// .datファイル関連
/// </summary>
namespace Dat
{	
	/// <summary>
	/// .datファイルを作成,上書き保存
	/// </summary	
	/// <typeparam name="T">データの型</typeparam>
	template<typename T>
	class DATWriter
	{
	public:
		DATWriter() {};
		~DATWriter() {};

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
		/// <param name="T"        >書き込むデータ        </param>
		void Write(const std::string& fileName, const T& data)
		{
			// データをファイルに書き込む
			std::ofstream ofs(m_folderPath + fileName + DAT::kExtension, std::ios_base::binary);
			ofs.write((char*)&data, sizeof(T));
			ofs.close();
		}

	private:
		// フォルダーパス
		std::string m_folderPath;
	};
}
