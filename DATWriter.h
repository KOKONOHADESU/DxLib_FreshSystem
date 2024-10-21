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
	class DATWriter
	{
	public:
		DATWriter() {};
		~DATWriter() {};

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
		void Write(const std::string& fileName, const T& id, const U& data)
		{
			// �f�[�^���t�@�C���ɏ�������
			std::ofstream ofs(fileName, std::ios_base::binary);
			ofs.write((char*)&data, sizeof(U));
		}

		/// <summary>
		/// �f�[�^���󂯎��
		/// </summary>
		/// <param name="id">ID���w��</param>
		/// <returns        >�f�[�^  </returns>
		U GetData(const T& id)
		{
			return m_datData[id];
		}

	private:

		// �t�H���_�[�p�X
		std::string m_folderPath;

		// .dat�t�@�C���̓����f�[�^
		std::map<T, U> m_datData;

	};
}
