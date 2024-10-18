#pragma once

#include "SingletonBase.h"
#include "CSVCommon.h"

#include <vector>
#include <sstream>
#include <fstream>

/// <summary>
/// CSV�֘A
/// </summary>
namespace CSV
{
	/// <summary>
	/// CSV�t�@�C���������ݗp
	/// </summary>
	class CSVWriter : public SingletonBase<CSVWriter>
	{
	public:
		/// <summary>
		/// SingletonBase�N���X�̃A�N�Z�X�������� 
		/// </summary>
		friend class SingletonBase<CSVWriter>;

	private:
		CSVWriter() {};

	public:
		~CSVWriter() {};		

		/// <summary>
		/// �摜������K�w�̃t�H���_�[���w��
		/// </summary>
		/// <param name="writerFolderPath">�������ݗp�t�H���_�̃p�X�w��</param>
		void CSVFolderPath(const char* writerFolderPath)
		{
			m_filePath = writerFolderPath;
		}

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="fileName" >�������ރt�@�C���l�[��</param>
		/// <param name="wirteData">�������ޓ��e          </param>
		void Write(const std::string& fileName, const std::vector<std::vector<std::string>>& wirteData)
		{
			std::ofstream ofs(m_filePath + fileName + kExtension);

			// �t�@�C�����������J���ꂽ���m�F
			if (!ofs.is_open())
			{
				return;
			}

			// �f�[�^��CSV�`���Ńt�@�C���ɏ�������
			for (const auto& row : wirteData)
			{
				for (int i = 0; i < static_cast<int>(row.size()); ++i)
				{
					ofs << row[i];

					if (i != row.size() - 1)
					{
						// �J���}�ŃZ������؂�
						ofs << ",";
					}
				}

				// ���s�ōs����؂�
				ofs << "\n";
			}

			// �t�@�C�������
			ofs.close();
		}

	private:

		// �t�@�C���p�X�L�^
		std::string m_filePath;
	};
}

