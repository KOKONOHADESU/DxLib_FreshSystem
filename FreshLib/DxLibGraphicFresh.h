#pragma once

#include "SingletonBase.h"

#include "Vec2.h"

#include <string>
#include <vector>
#include <map>
#include <DxLib.h>

/// <summary>
/// 画像関連
/// </summary>
namespace Graphic
{
	/// <summary>
	/// 画像管理
	/// </summary>
	/// <typeparam name="T">どのシーンで画像を読み込むかをしているする型</typeparam>
	/// <typeparam name="U">画像にIDをつける場合の型                    </typeparam>
	template <typename T , typename U>
	class DxLibGraphicFresh : public SingletonBase<DxLibGraphicFresh<T, U>>
	{
	private:
		// 画像データ
		struct GrahicData
		{
			U id{};                // ID
			std::string graphPath; // 画像パス
			int handle = -1;       // 画像データ
			Vec2<float> size;      // 画像サイズ
			T scene{};             // 使用するシーン
			bool isNoEnd = false;  // どのシーンでもメモリを解放しない場合
		};

	public:
		// SingletonBaseクラスのアクセスを許可する
		friend class SingletonBase<DxLibGraphicFresh<T, U>>;

	private:
		DxLibGraphicFresh() {};

	public:
		~DxLibGraphicFresh()
		{
			for (int i = 0; i < m_graphData.size(); i++)
			{
				// メモリの解放
				DeleteGraph(m_graphData[i].handle);
				m_graphData[i].handle = -1;
			}
		};

	public:		
		/// <summary>
		/// 画像がある階層のフォルダーを指定
		/// </summary>
		/// <param name="grahicFolderPath"></param>
		void GraphicFolderPath(const char* grahicFolderPath)
		{
			m_folderPath = grahicFolderPath;
		}

		/// <summary>
		/// 画像読み込み
		/// </summary>
		/// <param name="id"       >画像ID                        </param>
		/// <param name="scene"    >画像を使用するシーン          </param>
		/// <param name="filePath" >画像パス                      </param>
		/// <param name="extension">拡張子                        </param>
		/// <param name="isNoEnd"  >複数のシーンで使用するかどうか</param>
		/// <returns               >false : 失敗 , true : 成功    </returns>
		bool Add(const U& id, const T& scene, const char* filePath, const char* extension, bool isNoEnd = false)
		{
			// データ取得用
			GrahicData data{};

			// IDの記録
			data.id = id;

			// 画像パスの記録
			data.graphPath = m_folderPath + filePath + extension;

			// ここではまだ画像ロードしない
			data.handle = -1;
		
			// 使用するシーン
			data.scene = scene;

			// 画像のサイズを取得
			data.size.x = 0.0f;
			data.size.y = 0.0f;

			// 複数のシーンで使用するかどうか
			data.isNoEnd = isNoEnd;

			// 画像の追加
			m_graphData.push_back(data);

			// 読み込み成功したら
			return true;
		}

		/// <summary>
		/// 現在のシーンを確認する
		/// </summary>
		/// <param name="scene">現在のシーン</param>
		void SceneInput(const T& scene)
		{
			// すべての画像データを確認
			for (int i = 0; i < m_graphData.size(); i++)
			{		
				// 特定の画像データを確認
				// 画像ロードしていないデータを確認
				if (m_graphData[i].scene == scene && m_graphData[i].handle == -1)
				{
					// 画像のロード
					m_graphData[i].handle = LoadGraph(m_graphData[i].graphPath.c_str());

					// 読み込み失敗したら
					if (m_graphData[i].handle == -1)continue;

					continue;
				}
				// 他シーンで使用する画像の場合
				else if (m_graphData[i].scene != scene && !m_graphData[i].isNoEnd)
				{
					// 画像データが入っていた場合
					if (m_graphData[i].handle != -1)
					{					
						// メモリの解放
						DeleteGraph(m_graphData[i].handle);
						m_graphData[i].handle = -1;

						continue;
					}
				}
			}			
		}

		/// <summary>
		/// 画像ハンドルを渡す
		/// </summary>
		/// <param name="id">画像ID        </param>
		/// <returns        >ハンドルデータ</returns>
		int GetHandle(const U& id)
		{
			int handle = -1;

			// すべての画像を確認
			for (int i = 0; i < m_graphData.size(); i++)
			{
				// 同じIDを探す
				if (m_graphData[i].id == id)
				{
					// 同じIDが見つかったらループを終了
					handle = m_graphData[i].handle;
					break;
				}
				
			}

			return handle;
		}

		/// <summary>
		/// 画像サイズを渡す
		/// </summary>
		/// <param name="id">画像ID      </param>
		/// <returns        >サイズを渡す</returns>
		Vec2<float> GetSize(const U& id)
		{
			Vec2<float> size = Vec2<float>(0.0f,0.0f);

			// すべての画像を確認
			for (int i = 0; i < m_graphData.size(); i++)
			{
				// 同じIDを探す
				if (m_graphData[i].id == id)
				{
					// サイズを取得
					GetGraphSizeF(m_graphData[i].handle, &m_graphData[i].size.x, &m_graphData[i].size.y);

					// 同じIDが見つかったらループを終了
					return m_graphData[i].size;
				}
			}

			return Vec2<float>(0.0f, 0.0f);
		}

	private:
		// 画像データ				
		std::vector<GrahicData>m_graphData;

		// フォルダパス記録
		std::string m_folderPath;
	};
}


