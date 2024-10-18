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
			U id;                  // ID
			std::string graphPath; // �摜�p�X
			int handle;            // �摜�f�[�^
			Vec2 size;             // �摜�T�C�Y
			T scene;               // �g�p����V�[��
			bool isNoEnd;          // �ǂ̃V�[���ł���������������Ȃ��ꍇ
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
			m_filePath = grahicFolderPath;
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

			// ID�̋L�^
			data.id = id;

			// �摜�p�X�̋L�^
			data.graphPath = m_filePath + filePath + extension;

			// �����̃V�[���ŉ摜���g�p����ꍇ
			if (isNoEnd)
			{
				// �摜�̃��[�h
				data.handle = LoadGraph(data.graphPath.c_str());
			}
			else
			{
				// �����ł͂܂��摜���[�h���Ȃ�
				data.handle = -1;
			}
		
			// �g�p����V�[��
			data.scene = scene;

			// �摜�̃T�C�Y���擾
			GetGraphSizeF(data.handle, &data.size.x, &data.size.y);

			// �����̃V�[���Ŏg�p���邩�ǂ���
			data.isNoEnd = isNoEnd;

			// �摜�̒ǉ�
			m_graphData.push_back(data);

			// �ǂݍ��ݐ���������
			return true;
		}

		/// <summary>
		/// ���݂̃V�[�����m�F����
		/// </summary>
		/// <param name="scene">���݂̃V�[��</param>
		void SceneInput(const T& scene)
		{
			// ���ׂẲ摜�f�[�^���m�F
			for (int i = 0; i < m_graphData.size(); i++)
			{		
				// ����̉摜�f�[�^���m�F
				// �摜���[�h���Ă��Ȃ��f�[�^���m�F
				if (m_graphData[i].scene == scene && m_graphData[i].handle == -1)
				{
					// �摜�̃��[�h
					m_graphData[i].handle = LoadGraph(m_graphData[i].graphPath.c_str());

					// �ǂݍ��ݎ��s������
					if (m_graphData[i].handle == -1)return;	

					continue;
				}
				// ���V�[���Ŏg�p����摜�̏ꍇ
				else if (m_graphData[i].scene != scene && !m_graphData[i].isNoEnd)
				{
					// �摜�f�[�^�������Ă����ꍇ
					if (m_graphData[i].handle != -1)
					{					
						// �������̉��
						DeleteGraph(m_graphData[i].handle);
						m_graphData[i].handle = -1;

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
			int handle = -1;

			// ���ׂẲ摜���m�F
			for (int i = 0; i < m_graphData.size(); i++)
			{
				// ����ID��T��
				if (m_graphData[i].id == id)
				{
					// ����ID�����������烋�[�v���I��
					handle = m_graphData[i].handle;
					break;
				}
				
			}

			return handle;
		}

		/// <summary>
		/// �摜�T�C�Y��n��
		/// </summary>
		/// <param name="id">�摜ID      </param>
		/// <returns        >�T�C�Y��n��</returns>
		Vec2 GetSize(const U& id)
		{
			Vec2 size = Vec2(0.0f,0.0f);

			// ���ׂẲ摜���m�F
			for (int i = 0; i < m_graphData.size(); i++)
			{
				// ����ID��T��
				if (m_graphData[i].id == id)
				{
					// ����ID�����������烋�[�v���I��
					size = m_graphData[i].size;
					break;
				}

			}

			return size;
		}

	private:
		// �摜�f�[�^				
		std::vector<GrahicData>m_graphData;

		// �t�@�C���p�X�L�^
		std::string m_filePath;
	};
}


