#pragma once

#include "SingletonBase.h"

#include <map>
#include <string>

/// <summary>
/// �t�H���g�֘A
/// </summary>
namespace Font
{
	/// <summary>
	/// �t�H���g�ǂݍ���
	/// </summary>
	/// <typeparam name="T">ID    </typeparam>
	/// <typeparam name="U">�V�[��</typeparam>
	template<typename T , typename U>
	class DxLibFontFresh : public SingletonBase<DxLibFontFresh>
	{
	private:
		// �t�H���g�f�[�^
		struct FontData
		{
			U scene;              // �g�p�V�[��
			int handle;           // �n���h���f�[�^
			std::string filePath; // �t�@�C���p�X
			std::string fontName; // �t�H���g�̖��O
			bool isNoEnd;         // ���ׂẴV�[���Ŏg�p���邩�ǂ���
		};

	public:
		// SingletonBase�N���X�̃A�N�Z�X��������
		friend class SingletonBase<DxLibFontFresh>;

		~DxLibFontFresh() {};
	private:
		DxLibFontFresh() {};

	public:
		/// <summary>
		/// �t�H���g�f�[�^������K�w�̃t�H���_�[���w��
		/// </summary>
		/// <param name="grahicFolderPath"></param>
		void FontFolderPath(const char* fontFolderPath)
		{
			m_folderPath = fontFolderPath;
		}

		/// <summary>
		/// �t�H���g��ǉ�����
		/// </summary>
		/// <param name="id"       >ID                            </param>
		/// <param name="scene"    >�g�p����V�[��                </param>
		/// <param name="fileName" >�t�@�C���̖��O                </param>
		/// <param name="extension">�g���q                        </param>
		/// <param name="fontName" >�t�H���g�̖��O                </param>
		/// <param name="isNoEnd"  >���̃V�[���ł��g�p���邩�ǂ���</param>
		/// <returns               >true : ���� , false : ���s    </returns>
		bool Add(const T& id, const U& scene, const std::string& fileName, const std::string& extension, const std::string& fontName, bool isNoEnd = false)
		{
			FontData data{};

			// �g�p����V�[��
			data.scene = scene;

			// ���̃V�[���ł��g�p����ꍇ�͑����Ƀ������m��
			// ���Ȃ��ꍇ�� -1 ����
			if (isNoEnd)
			{
				// �t�@�C���̖��O���܂Ƃ߂�
				std::string filePath = m_folderPath + fileName + extension;

				// �t�H���g�̓ǂݍ���
				AddFontResourceEx(filePath.c_str(), FR_PRIVATE, NULL);
				data.handle = CreateFontToHandle(fontName.c_str(), 40, 3, DX_FONTTYPE_NORMAL, DX_CHARSET_DEFAULT);
			}
			else
			{
				// �ǂݍ��܂Ȃ�
				data.handle = -1;
			}

			// �t�@�C���p�X
			data.fileName = m_folderPath + fileName + extension;

			// �t�H���g�̖��O
			data.fontName = fontName;

			// ���̃V�[���ł��g�p���邩�ǂ���
			data.isNoEnd = isNoEnd;

			// �f�[�^�̒ǉ�
			m_font[id] = data;
		}

	private:

		// �t�H���g�Ǘ��f�[�^
		std::map<T, FontData> m_font;

		// �t�H���g������t�H���_�[
		std::string m_folderPath;

	};
}