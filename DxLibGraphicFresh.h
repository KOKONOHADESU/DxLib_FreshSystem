#pragma once

#include <string>
#include <vector>
#include <DxLib.h>

namespace
{
	class DxLibGraphicFresh
	{
	private:
		struct GrahicData
		{
			int handle;
			bool isSoftData;
			
			int sizeX;
			int sizeY;
		};
	public:
		DxLibGraphicFresh() {};
		~DxLibGraphicFresh() {};
	
		bool LoadSoftGrahic(const std::string& filePath)
		{
			int image = LoadSoftImage(filePath.c_str());
			// 画像のサイズを取得
			int x = 0;
			int y = 0;
			GetSoftImageSize(image, &x, &y);

			GrahicData data
			{
				image,
				true,
				x,
				y
			};

			// 画像の読み込み
			m_handle.push_back(data);

			// 読み込み失敗したら
			if (m_handle.back().handle == -1)return false;

			// 読み込み成功したら
			return true;
		}

		bool LoadGrahic(std::string filePath)
		{
			int image = LoadGraph(filePath.c_str());
			// 画像のサイズを取得
			int x = 0;
			int y = 0;
			GetGraphSize(image, &x, &y);

			GrahicData data
			{
				image,
				true,
				x,
				y
			};


			// 画像の読み込み
			m_handle.push_back(data);

			// 読み込み失敗したら
			if (m_handle.back().handle == -1)return false;

			// 読み込み成功したら
			return true;
		}

		void EndGrahics()
		{
			for (int i = 0; i < m_handle.size(); i++)
			{
				if(m_handle[i].isSoftData)
				{
					DeleteSoftImage(m_handle[i].handle);
				}
				else
				{
					DeleteGraph(m_handle[i].handle);
				}
			}
		}


		void DrawGraphic()
		{
			int r, g, b, a;
			// 画像の色を１ドットづつ参照して DrawBox で３倍の大きさにして描画
			for (int i = 0; i < m_handle[0].sizeY; i++)
			{
				for (int j = 0; j < m_handle[0].sizeX; j++)
				{
					// １ドットの色を取得
					GetPixelSoftImage(m_handle[0].handle, j, i, &r, &g, &b, &a);

					// DrawBox で描画
					DrawBox(j * 1, i * 1, j * 1 + 1, i * 1 + 1, GetColor(r, g, b), true);
				}
			}

			DrawPixelSoftImage(m_handle[0].handle, 100, 100, 255, 255, 255, 0);
		}

	private:
		std::vector<GrahicData> m_handle;
	};
}

