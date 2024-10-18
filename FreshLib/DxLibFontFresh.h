#pragma once

#include "SingletonBase.h"

#include <map>
#include <string>

/// <summary>
/// フォント関連
/// </summary>
namespace Font
{
	/// <summary>
	/// フォント読み込み
	/// </summary>
	/// <typeparam name="T">ID    </typeparam>
	/// <typeparam name="U">シーン</typeparam>
	template<typename T , typename U>
	class DxLibFontFresh : public SingletonBase<DxLibFontFresh>
	{
	private:
		// フォントデータ
		struct FontData
		{
			U scene;              // 使用シーン
			int handle;           // ハンドルデータ
			std::string filePath; // ファイルパス
			std::string fontName; // フォントの名前
			bool isNoEnd;         // すべてのシーンで使用するかどうか
		};

	public:
		// SingletonBaseクラスのアクセスを許可する
		friend class SingletonBase<DxLibFontFresh>;

		~DxLibFontFresh() {};
	private:
		DxLibFontFresh() {};

	public:
		/// <summary>
		/// フォントデータがある階層のフォルダーを指定
		/// </summary>
		/// <param name="grahicFolderPath"></param>
		void FontFolderPath(const char* fontFolderPath)
		{
			m_folderPath = fontFolderPath;
		}

		/// <summary>
		/// フォントを追加する
		/// </summary>
		/// <param name="id"       >ID                            </param>
		/// <param name="scene"    >使用するシーン                </param>
		/// <param name="fileName" >ファイルの名前                </param>
		/// <param name="extension">拡張子                        </param>
		/// <param name="fontName" >フォントの名前                </param>
		/// <param name="isNoEnd"  >他のシーンでも使用するかどうか</param>
		/// <returns               >true : 成功 , false : 失敗    </returns>
		bool Add(const T& id, const U& scene, const std::string& fileName, const std::string& extension, const std::string& fontName, bool isNoEnd = false)
		{
			FontData data{};

			// 使用するシーン
			data.scene = scene;

			// 他のシーンでも使用する場合は即座にメモリ確保
			// しない場合は -1 を代入
			if (isNoEnd)
			{
				// ファイルの名前をまとめる
				std::string filePath = m_folderPath + fileName + extension;

				// フォントの読み込み
				AddFontResourceEx(filePath.c_str(), FR_PRIVATE, NULL);
				data.handle = CreateFontToHandle(fontName.c_str(), 40, 3, DX_FONTTYPE_NORMAL, DX_CHARSET_DEFAULT);
			}
			else
			{
				// 読み込まない
				data.handle = -1;
			}

			// ファイルパス
			data.fileName = m_folderPath + fileName + extension;

			// フォントの名前
			data.fontName = fontName;

			// 他のシーンでも使用するかどうか
			data.isNoEnd = isNoEnd;

			// データの追加
			m_font[id] = data;
		}

	private:

		// フォント管理データ
		std::map<T, FontData> m_font;

		// フォントがあるフォルダー
		std::string m_folderPath;

	};
}