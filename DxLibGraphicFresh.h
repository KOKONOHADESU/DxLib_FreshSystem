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
			// �摜�̃T�C�Y���擾
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

			// �摜�̓ǂݍ���
			m_handle.push_back(data);

			// �ǂݍ��ݎ��s������
			if (m_handle.back().handle == -1)return false;

			// �ǂݍ��ݐ���������
			return true;
		}

		bool LoadGrahic(std::string filePath)
		{
			int image = LoadGraph(filePath.c_str());
			// �摜�̃T�C�Y���擾
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


			// �摜�̓ǂݍ���
			m_handle.push_back(data);

			// �ǂݍ��ݎ��s������
			if (m_handle.back().handle == -1)return false;

			// �ǂݍ��ݐ���������
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
			// �摜�̐F���P�h�b�g�ÂQ�Ƃ��� DrawBox �łR�{�̑傫���ɂ��ĕ`��
			for (int i = 0; i < m_handle[0].sizeY; i++)
			{
				for (int j = 0; j < m_handle[0].sizeX; j++)
				{
					// �P�h�b�g�̐F���擾
					GetPixelSoftImage(m_handle[0].handle, j, i, &r, &g, &b, &a);

					// DrawBox �ŕ`��
					DrawBox(j * 1, i * 1, j * 1 + 1, i * 1 + 1, GetColor(r, g, b), true);
				}
			}

			DrawPixelSoftImage(m_handle[0].handle, 100, 100, 255, 255, 255, 0);
		}

	private:
		std::vector<GrahicData> m_handle;
	};
}

