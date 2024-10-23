#pragma once

#include "SingletonBase.h"
#include "DATCommon.h"

#include <string>
#include <map>

/// <summary>
/// .dat�t�@�C���֘A
/// </summary>
namespace DAT
{	
	/// <summary>
	/// .dat�t�@�C�����쐬,�㏑���ۑ�
	/// </summary>
	/// <typeparam name="T">ID            </typeparam>
	/// <typeparam name="U">�g�p����V�[��</typeparam>
	/// <typeparam name="V">�f�[�^�̌^    </typeparam>
	template<typename T, typename U , typename V>
	class DATReader : public SingletonBase<DATReader<T, U, V>>
	{
	private:
		/// <summary>
		/// �ǂݍ��ݗp�f�[�^
		/// </summary>
		struct ReaderData
		{
			U scene{};            // �V�[��	
			std::string filePath; // �p�X			
			V data{};             // .data�t�@�C���f�[�^
			bool isNoEnd = false; // �ǂ̃V�[���ł��g�p����ꍇ
		};
	public:
		// SingletonBase�N���X�̃A�N�Z�X��������
		friend class SingletonBase<DATReader<T, U, V>>;
	private:
		DATReader() {};
	public:
		~DATReader() {};

		/// <summary>
		/// .dat�t�@�C��������K�w�̃t�H���_�[���w��
		/// </summary>
		/// <param name="writerFolderPath">�������ݗp�t�H���_�̃p�X�w��</param>
		void DATFolderPath(const char* writerFolderPath)
		{
			m_folderPath = writerFolderPath;
		}

		/// <summary>
		/// �ǂݍ���
		/// </summary>
		/// <param name="T"        >ID                              </param>
		/// <param name="U"        >�g�p����V�[��                  </param>
		/// <param name="fileName" >�������ރt�@�C���l�[��          </param>
		/// <param name="isNoEnd"  >�ǂ̃V�[���ł��폜���Ȃ����ǂ���</param>
		void Add(const T& id, const U& scene, const std::string& fileName, bool isNoEnd = false)
		{			
			ReaderData data{};

			// �g�p����V�[��
			data.scene = scene;

			// �t�@�C���p�X
			data.filePath = m_folderPath + fileName + DAT::kExtension;

			// �ǂ̃V�[���ł��g�p���邩�ǂ���
			// ����ꍇ�����Ƀ��[�h
			if (isNoEnd)
			{				
				// �f�[�^���t�@�C���ɏ�������
				std::ifstream ifs;
				ifs.open(data.filePath, std::ios_base::binary);
				ifs.read((char*)&data.data, sizeof(V));
				ifs.close();
			}
			else
			{
				// ������
				data.data = V();
			}

			// ���̃V�[���ł��g�p���邩�ǂ���
			data.isNoEnd = isNoEnd;

			// �f�[�^�̐ݒ芮��
			m_datData[id] = data;
		}

		/// <summary>
		/// ���݂̃V�[�����m�F����
		/// </summary>
		/// <param name="scene">���݂̃V�[��</param>
		void SceneInput(const U& scene)
		{
			// ���ׂẴf�[�^���m�F
			for (auto& data : m_datData)
			{
				// second��ReaderData�ɃA�N�Z�X
				ReaderData& datData = data.second;

				// ����̃f�[�^���m�F
				// ���[�h���Ă��Ȃ��f�[�^���m�F
				if (datData.scene == scene)
				{
					// �f�[�^���t�@�C���ɏ�������
					std::ifstream ifs;
					ifs.open(datData.filePath, std::ios_base::binary);
					ifs.read((char*)&datData.data, sizeof(V));
					ifs.close();

					continue;
				}
				else
				{
					// ������
					datData.data = V();

					continue;
				}
			}
		}
		
		/// <summary>
		/// �f�[�^��n��
		/// </summary>
		/// <param name="U">ID    </param>
		/// <returns       >�f�[�^</returns>
		V GetData(const U& id)
		{
			// �}�b�v����ID�ɑΉ�����f�[�^������
			auto it = m_datData.find(id);

			// ID������������n���h����Ԃ�
			if (it != m_datData.end())
			{
				return it->second.data;
			}

			return V();
		}

	private:
		// �t�H���_�[�p�X
		std::string m_folderPath;

		// .dat�t�@�C���̓����f�[�^
		std::map<T, ReaderData> m_datData;

	};
}
