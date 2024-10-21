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
	/// </summary	
	/// <typeparam name="T">�f�[�^�̌^</typeparam>
	template<typename T>
	class DATWriter : public SingletonBase<DATWriter<T>>
	{
	public:
		// SingletonBase�N���X�̃A�N�Z�X��������
		friend class SingletonBase<DATWriter<T>>;
	private:
		DATWriter() {};
	public:
		~DATWriter() {};

		/// <summary>
		/// .dat�t�@�C��������K�w�̃t�H���_�[���w��
		/// </summary>
		/// <param name="writerFolderPath">�������ݗp�t�H���_�̃p�X�w��</param>
		void DATFolderPath(const char* writerFolderPath)
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
