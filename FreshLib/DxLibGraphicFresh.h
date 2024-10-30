#pragma once

#include "SingletonBase.h"

#include "Vec2.h"

#include <string>
#include <vector>
#include <map>
#include <DxLib.h>

/// <summary>
/// �摜�֘A
/// </summary>
namespace Graphic
{
	/// <summary>
	/// �摜�Ǘ�
	/// </summary>
	/// <typeparam name="T">�ǂ̃V�[���ŉ摜��ǂݍ��ނ������Ă��邷��^</typeparam>
	/// <typeparam name="U">�摜��ID������ꍇ�̌^                    </typeparam>
	template <typename T , typename U>
	class DxLibGraphicFresh : public SingletonBase<DxLibGraphicFresh<T, U>>
	{
	private:
		// �摜�f�[�^
		struct GrahicData
		{			
			std::string graphPath; // �摜�p�X
			int handle = -1;       // �摜�f�[�^
			Vec2<float> size;      // �摜�T�C�Y
			T scene{};             // �g�p����V�[��
			bool isNoEnd = false;  // �ǂ̃V�[���ł���������������Ȃ��ꍇ
		};

	public:
		// SingletonBase�N���X�̃A�N�Z�X��������
		friend class SingletonBase<DxLibGraphicFresh<T, U>>;

	private:
		DxLibGraphicFresh() {};

	public:
		~DxLibGraphicFresh()
		{
			for (int i = 0; i < m_graphData.size(); i++)
			{
				// �������̉��
				DeleteGraph(m_graphData[i].handle);
				m_graphData[i].handle = -1;
			}
		};

	public:		
		/// <summary>
		/// �摜������K�w�̃t�H���_�[���w��
		/// </summary>
		/// <param name="grahicFolderPath"></param>
		void GraphicFolderPath(const char* grahicFolderPath)
		{
			m_folderPath = grahicFolderPath;
		}

		/// <summary>
		/// �摜�ǂݍ���
		/// </summary>
		/// <param name="id"       >�摜ID                        </param>
		/// <param name="scene"    >�摜���g�p����V�[��          </param>
		/// <param name="filePath" >�摜�p�X                      </param>
		/// <param name="extension">�g���q                        </param>
		/// <param name="isNoEnd"  >�����̃V�[���Ŏg�p���邩�ǂ���</param>
		/// <returns               >false : ���s , true : ����    </returns>
		bool Add(const U& id, const T& scene, const char* filePath, const char* extension, bool isNoEnd = false)
		{
			// �f�[�^�擾�p
			GrahicData data{};

			// �摜�p�X�̋L�^
			data.graphPath = m_folderPath + filePath + extension;

			// �����ł͂܂��摜���[�h���Ȃ�
			data.handle = -1;
		
			// �g�p����V�[��
			data.scene = scene;

			// �摜�̃T�C�Y���擾
			data.size.x = 0.0f;
			data.size.y = 0.0f;

			// �����̃V�[���Ŏg�p���邩�ǂ���
			data.isNoEnd = isNoEnd;

			// �摜�̒ǉ�
			m_graphData[id] = data;

			// �ǂݍ��ݐ���������
			return true;
		}

		/// <summary>
		/// ���݂̃V�[�����m�F����
		/// </summary>
		/// <param name="scene">���݂̃V�[��</param>
		void SceneInput(const T& scene)
		{
			// ���ׂẴf�[�^���m�F
			for (auto& graphic : m_graphData)
			{
				// second��FontData�ɃA�N�Z�X
				GrahicData& graphicData = graphic.second;

				// ����̃f�[�^���m�F
				// ���[�h���Ă��Ȃ��f�[�^���m�F
				if (graphicData.scene == scene && graphicData.handle == -1)
				{
					// �ǂݍ���
					graphicData.handle = LoadGraph(graphicData.graphPath.c_str());

					// �ǂݍ��ݎ��s������
					if (graphicData.handle == -1) return;

					continue;
				}
				// ���V�[���Ŏg�p����ꍇ
				else if (graphicData.scene != scene && !graphicData.isNoEnd)
				{
					// �f�[�^�������Ă����ꍇ
					if (graphicData.handle != -1)
					{
						// �������
						DeleteGraph(graphicData.handle);
						graphicData.handle = -1;

						continue;
					}
				}
			}
		}

		/// <summary>
		/// �摜�n���h����n��
		/// </summary>
		/// <param name="id">�摜ID        </param>
		/// <returns        >�n���h���f�[�^</returns>
		int GetHandle(const U& id)
		{
			// ID�ɑΉ�����f�[�^������
			auto it = m_graphData.find(id);

			if (it != m_graphData.end())
			{
				return it->second.handle;
			}

			return -1;			
		}

		/// <summary>
		/// �摜�T�C�Y��n��
		/// </summary>
		/// <param name="id">�摜ID      </param>
		/// <returns        >�T�C�Y��n��</returns>
		Vec2<float> GetSize(const U& id)
		{					
			GetGraphSizeF(m_graphData[id].handle, &m_graphData[id].size.x, &m_graphData[id].size.y);

			return m_graphData[id].size;
		}

		/// <summary>
		/// �z��m�F
		/// </summary>
		/// <returns>�z��̐���n��</returns>
		int GetArrayNum()
		{
			return static_cast<int>(m_graphData.size());
		}

		/// <summary>
		/// �ǂݍ���ł���摜�̐���n��
		/// </summary>
		/// <returns>�ǂݍ���ł���摜�̐�</returns>
		int GetHandleNum()
		{
			int graphicNum = 0;
			for (auto& graphic : m_graphData)
			{
				// second�ŃA�N�Z�X
				GrahicData& graphicData = graphic.second;

				if (graphicData.handle != -1)
				{
					graphicNum++;
				}
			}

			return graphicNum;
		}

	private:
		// �摜�f�[�^					
		std::map<T, GrahicData>m_graphData;

		// �t�H���_�p�X�L�^
		std::string m_folderPath;
	};
}


