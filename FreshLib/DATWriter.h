#pragma once

#include <string>
#include <map>

#include "DATCommon.h"

/// <summary>
/// .dat�t�@�C���֘A
/// </summary>
namespace Dat
{	
	/// <summary>
	/// .dat�t�@�C�����쐬,�㏑���ۑ�
	/// </summary	
	/// <typeparam name="T">�f�[�^�̌^</typeparam>
	template<typename T>
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
		/// <param name="T"        >�������ރf�[�^        </param>
		void Write(const std::string& fileName, const T& data)
		{
			// �f�[�^���t�@�C���ɏ�������
			std::ofstream ofs(m_folderPath + fileName + DAT::kExtension, std::ios_base::binary);
			ofs.write((char*)&data, sizeof(T));
			ofs.close();
		}

	private:
		// �t�H���_�[�p�X
		std::string m_folderPath;
	};
}
