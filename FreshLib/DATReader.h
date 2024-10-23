#pragma once

#include "SingletonBase.h"
#include "DATCommon.h"

#include <string>
#include <map>

/// <summary>
/// .datファイル関連
/// </summary>
namespace DAT
{	
	/// <summary>
	/// .datファイルを作成,上書き保存
	/// </summary>
	/// <typeparam name="T">ID            </typeparam>
	/// <typeparam name="U">使用するシーン</typeparam>
	/// <typeparam name="V">データの型    </typeparam>
	template<typename T, typename U , typename V>
	class DATReader : public SingletonBase<DATReader<T, U, V>>
	{
	private:
		/// <summary>
		/// 読み込み用データ
		/// </summary>
		struct ReaderData
		{
			U scene{};            // シーン	
			std::string filePath; // パス			
			V data{};             // .dataファイルデータ
			bool isNoEnd = false; // どのシーンでも使用する場合
		};
	public:
		// SingletonBaseクラスのアクセスを許可する
		friend class SingletonBase<DATReader<T, U, V>>;
	private:
		DATReader() {};
	public:
		~DATReader() {};

		/// <summary>
		/// .datファイルがある階層のフォルダーを指定
		/// </summary>
		/// <param name="writerFolderPath">書き込み用フォルダのパス指定</param>
		void DATFolderPath(const char* writerFolderPath)
		{
			m_folderPath = writerFolderPath;
		}

		/// <summary>
		/// 読み込み
		/// </summary>
		/// <param name="T"        >ID                              </param>
		/// <param name="U"        >使用するシーン                  </param>
		/// <param name="fileName" >書き込むファイルネーム          </param>
		/// <param name="isNoEnd"  >どのシーンでも削除しないかどうか</param>
		void Add(const T& id, const U& scene, const std::string& fileName, bool isNoEnd = false)
		{			
			ReaderData data{};

			// 使用するシーン
			data.scene = scene;

			// ファイルパス
			data.filePath = m_folderPath + fileName + DAT::kExtension;

			// どのシーンでも使用するかどうか
			// する場合すぐにロード
			if (isNoEnd)
			{				
				// データをファイルに書き込む
				std::ifstream ifs;
				ifs.open(data.filePath, std::ios_base::binary);
				ifs.read((char*)&data.data, sizeof(V));
				ifs.close();
			}
			else
			{
				// 初期化
				data.data = V();
			}

			// 他のシーンでも使用するかどうか
			data.isNoEnd = isNoEnd;

			// データの設定完了
			m_datData[id] = data;
		}

		/// <summary>
		/// 現在のシーンを確認する
		/// </summary>
		/// <param name="scene">現在のシーン</param>
		void SceneInput(const U& scene)
		{
			// すべてのデータを確認
			for (auto& data : m_datData)
			{
				// secondでReaderDataにアクセス
				ReaderData& datData = data.second;

				// 特定のデータを確認
				// ロードしていないデータを確認
				if (datData.scene == scene)
				{
					// データをファイルに書き込む
					std::ifstream ifs;
					ifs.open(datData.filePath, std::ios_base::binary);
					ifs.read((char*)&datData.data, sizeof(V));
					ifs.close();

					continue;
				}
				else
				{
					// 初期化
					datData.data = V();

					continue;
				}
			}
		}
		
		/// <summary>
		/// データを渡す
		/// </summary>
		/// <param name="U">ID    </param>
		/// <returns       >データ</returns>
		V GetData(const U& id)
		{
			// マップからIDに対応するデータを検索
			auto it = m_datData.find(id);

			// IDが見つかったらハンドルを返す
			if (it != m_datData.end())
			{
				return it->second.data;
			}

			return V();
		}

	private:
		// フォルダーパス
		std::string m_folderPath;

		// .datファイルの内部データ
		std::map<T, ReaderData> m_datData;

	};
}
