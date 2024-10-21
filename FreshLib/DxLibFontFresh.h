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
	class DxLibFontFresh : public SingletonBase<DxLibFontFresh<T, U>>
	{
	private:
		// フォントデータ
		struct FontData
		{
			U scene{};            // 使用シーン
			int handle = -1;      // ハンドルデータ
			std::string filePath; // ファイルパス
			std::string fontName; // フォントの名前
			int size = -1;        // サイズ
			int thick = -1;       // 厚さ
			bool isNoEnd = false; // すべてのシーンで使用するかどうか
		};

	public:
		// SingletonBaseクラスのアクセスを許可する
		friend class SingletonBase<DxLibFontFresh<T, U>>;

		~DxLibFontFresh() {};
	private:
		DxLibFontFresh() {};

	public:
		/// <summary>
		/// フォントデータがある階層のフォルダーを指定
		/// </summary>
		/// <param name="grahicFolderPath">フォルダーパス</param>
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
		/// <param name="size"     >フォントのサイズ              </param>
		/// <param name="thick"    >フォントの厚さ                </param>
		/// <param name="isNoEnd"  >他のシーンでも使用するかどうか</param>
		/// <returns               >true : 成功 , false : 失敗    </returns>
		bool Add(const T& id, const U& scene, const std::string& fileName, const std::string& extension, const std::string& fontName, int size = -1, int thick = -1, bool isNoEnd = false)
		{
			FontData data{};

			// 使用するシーン
			data.scene = scene;

			// シーンに左右されないデータを作成する場合
			if (isNoEnd)
			{
				// ファイルパスを作成
				const std::string filePath = m_folderPath + fileName + extension;

				// フォントの読み込み				
				if (AddFontResourceEx(filePath.c_str(), FR_PRIVATE, NULL) == 0)
				{
					return false;
				}

				// フォントハンドルを作成
				data.handle = CreateFontToHandle(fontName.c_str(), size, thick, DX_FONTTYPE_NORMAL, DX_CHARSET_DEFAULT);
				if (data.handle == -1)
				{
					return false;
				}
			}
			else
			{
				// 読み込まない場合
				data.handle = -1;
			}

			// ファイルパスを FontData に格納
			data.filePath = m_folderPath + fileName + extension;

			// フォントの名前
			data.fontName = fontName;

			// サイズ
			data.size = size;

			// 厚さ
			data.thick = thick;

			// 他のシーンでも使用するかどうか
			data.isNoEnd = isNoEnd;

			// データをマップに追加
			m_font[id] = data;

			// 成功時は true を返す
			return true;  
		}

		/// <summary>
		/// 現在のシーンを確認する
		/// </summary>
		/// <param name="scene">現在のシーン</param>
		void SceneInput(const T& scene)
		{
			// すべてのデータを確認
			for (auto& fontPair : m_font)
			{
				// secondでFontDataにアクセス
				FontData& fontData = fontPair.second;  

				// 特定のデータを確認
				// ロードしていないデータを確認
				if (fontData.scene == scene && fontData.handle == -1)
				{
					// フォントの読み込み
					AddFontResourceEx(fontData.filePath.c_str(), FR_PRIVATE, NULL);
					fontData.handle = CreateFontToHandle(fontData.fontName.c_str(), fontData.size, fontData.thick, DX_FONTTYPE_NORMAL, DX_CHARSET_DEFAULT);

					// 読み込み失敗したら
					if (fontData.handle == -1) return;

					continue;
				}
				// 他シーンで使用する場合
				else if (fontData.scene != scene && !fontData.isNoEnd)
				{
					// データが入っていた場合
					if (fontData.handle != -1)
					{
						// 解放処理
						DeleteFontToHandle(fontData.handle);
						fontData.handle = -1;

						continue;
					}
				}
			}
		}

		/// <summary>
		/// 画像ハンドルを渡す
		/// </summary>
		/// <param name="id">ID            </param>
		/// <returns        >ハンドルデータ</returns>
		int GetHandle(const U& id)
		{
			// マップからIDに対応するフォントデータを検索
			auto it = m_font.find(id);

			// IDが見つかったらハンドルを返す、見つからなかったら -1 を返す
			if (it != m_font.end())
			{
				return it->second.handle;
			}
		}

	private:
		// フォント管理データ
		std::map<T, FontData> m_font;

		// フォントがあるフォルダー
		std::string m_folderPath;

	};
}