#pragma once

#include <string>
#include <map>

/// <summary>
/// .dat�t�@�C���֘A
/// </summary>
namespace Dat
{	
	/// <summary>
	/// .dat�t�@�C�����쐬,�㏑���ۑ�
	/// </summary>
	/// <typeparam name="T">ID        </typeparam>
	/// <typeparam name="U">�f�[�^�̌^</typeparam>
	template<typename T, typename U>
	class DATReader
	{
	public:
		DATReader() {};
		~DATReader() {};

		/// <summary>
		/// .dat�t�@�C��������K�w�̃t�H���_�[���w��
		/// </summary>
		/// <param name="writerFolderPath">�������ݗp�t�H���_�̃p�X�w��</param>
		void CSVFolderPath(const char* writerFolderPath)
		{
			m_folderPath = writerFolderPath;
		}

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="fileName" >�������ރt�@�C���l�[��</param>
		/// <param name="T"        >ID                    </param>
		/// <param name="U"        >�������ރf�[�^        </param>
		U Reader(const std::string& fileName, const T& id)
		{						 
			std::ifstream ifs;
			ifs.open(fileName.c_str(), std::ios_base::binary);
			// �f�[�^���t�@�C���ɏ�������
			ifs.read((char*)&m_datData[id], sizeof(U));
			ifs.close();

			return m_datData[id];
		}

		



	private:

		// �t�H���_�[�p�X
		std::string m_folderPath;

		// .dat�t�@�C���̓����f�[�^
		std::map<T, U> m_datData;

	};
}
