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
			m_graphData[id] = data;

			// 読み込み成功したら
			return true;
		}

		/// <summary>
		/// 現在のシーンを確認する
		/// </summary>
		/// <param name="scene">現在のシーン</param>
		void SceneInput(const T& scene)
		{
			// すべてのデータを確認
			for (auto& graphic : m_graphData)
			{
				// secondでFontDataにアクセス
				GrahicData& graphicData = graphic.second;

				// 特定のデータを確認
				// ロードしていないデータを確認
				if (graphicData.scene == scene && graphicData.handle == -1)
				{
					// 読み込み
					graphicData.handle = LoadGraph(graphicData.graphPath.c_str());

					// 読み込み失敗したら
					if (graphicData.handle == -1) return;

					continue;
				}
				// 他シーンで使用する場合
				else if (graphicData.scene != scene && !graphicData.isNoEnd)
				{
					// データが入っていた場合
					if (graphicData.handle != -1)
					{
						// 解放処理
						DeleteGraph(graphicData.handle);
						graphicData.handle = -1;

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
			// IDに対応するデータを検索
			auto it = m_graphData.find(id);

			if (it != m_graphData.end())
			{
				return it->second.handle;
			}

			return -1;			
		}

		/// <summary>
		/// 画像サイズを渡す
		/// </summary>
		/// <param name="id">画像ID      </param>
		/// <returns        >サイズを渡す</returns>
		Vec2<float> GetSize(const U& id)
		{					
			GetGraphSizeF(m_graphData[id].handle, &m_graphData[id].size.x, &m_graphData[id].size.y);

			return m_graphData[id].size;
		}

		/// <summary>
		/// 配列確認
		/// </summary>
		/// <returns>配列の数を渡す</returns>
		int GetArrayNum()
		{
			return static_cast<int>(m_graphData.size());
		}

		/// <summary>
		/// 読み込んでいる画像の数を渡す
		/// </summary>
		/// <returns>読み込んでいる画像の数</returns>
		int GetHandleNum()
		{
			int graphicNum = 0;
			for (auto& graphic : m_graphData)
			{
				// secondでアクセス
				GrahicData& graphicData = graphic.second;

				if (graphicData.handle != -1)
				{
					graphicNum++;
				}
			}

			return graphicNum;
		}

	private:
		// 画像データ					
		std::map<T, GrahicData>m_graphData;

		// フォルダパス記録
		std::string m_folderPath;
	};
}


