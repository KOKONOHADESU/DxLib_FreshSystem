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
	class DxLibFontFresh : public SingletonBase<DxLibFontFresh<T, U>>
	{
	private:
		// �t�H���g�f�[�^
		struct FontData
		{
			U scene{};            // �g�p�V�[��
			int handle = -1;      // �n���h���f�[�^
			std::string filePath; // �t�@�C���p�X
			std::string fontName; // �t�H���g�̖��O
			int size = -1;        // �T�C�Y
			int thick = -1;       // ����
			bool isNoEnd = false; // ���ׂẴV�[���Ŏg�p���邩�ǂ���
		};

	public:
		// SingletonBase�N���X�̃A�N�Z�X��������
		friend class SingletonBase<DxLibFontFresh<T, U>>;

		~DxLibFontFresh() {};
	private:
		DxLibFontFresh() {};

	public:
		/// <summary>
		/// �t�H���g�f�[�^������K�w�̃t�H���_�[���w��
		/// </summary>
		/// <param name="grahicFolderPath">�t�H���_�[�p�X</param>
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
		/// <param name="size"     >�t�H���g�̃T�C�Y              </param>
		/// <param name="thick"    >�t�H���g�̌���                </param>
		/// <param name="isNoEnd"  >���̃V�[���ł��g�p���邩�ǂ���</param>
		/// <returns               >true : ���� , false : ���s    </returns>
		bool Add(const T& id, const U& scene, const std::string& fileName, const std::string& extension, const std::string& fontName, int size = -1, int thick = -1, bool isNoEnd = false)
		{
			FontData data{};

			// �g�p����V�[��
			data.scene = scene;

			// �V�[���ɍ��E����Ȃ��f�[�^���쐬����ꍇ
			if (isNoEnd)
			{
				// �t�@�C���p�X���쐬
				const std::string filePath = m_folderPath + fileName + extension;

				// �t�H���g�̓ǂݍ���				
				if (AddFontResourceEx(filePath.c_str(), FR_PRIVATE, NULL) == 0)
				{
					return false;
				}

				// �t�H���g�n���h�����쐬
				data.handle = CreateFontToHandle(fontName.c_str(), size, thick, DX_FONTTYPE_NORMAL, DX_CHARSET_DEFAULT);
				if (data.handle == -1)
				{
					return false;
				}
			}
			else
			{
				// �ǂݍ��܂Ȃ��ꍇ
				data.handle = -1;
			}

			// �t�@�C���p�X�� FontData �Ɋi�[
			data.filePath = m_folderPath + fileName + extension;

			// �t�H���g�̖��O
			data.fontName = fontName;

			// �T�C�Y
			data.size = size;

			// ����
			data.thick = thick;

			// ���̃V�[���ł��g�p���邩�ǂ���
			data.isNoEnd = isNoEnd;

			// �f�[�^���}�b�v�ɒǉ�
			m_font[id] = data;

			// �������� true ��Ԃ�
			return true;  
		}

		/// <summary>
		/// ���݂̃V�[�����m�F����
		/// </summary>
		/// <param name="scene">���݂̃V�[��</param>
		void SceneInput(const T& scene)
		{
			// ���ׂẴf�[�^���m�F
			for (auto& fontPair : m_font)
			{
				// second��FontData�ɃA�N�Z�X
				FontData& fontData = fontPair.second;  

				// ����̃f�[�^���m�F
				// ���[�h���Ă��Ȃ��f�[�^���m�F
				if (fontData.scene == scene && fontData.handle == -1)
				{
					// �t�H���g�̓ǂݍ���
					AddFontResourceEx(fontData.filePath.c_str(), FR_PRIVATE, NULL);
					fontData.handle = CreateFontToHandle(fontData.fontName.c_str(), fontData.size, fontData.thick, DX_FONTTYPE_NORMAL, DX_CHARSET_DEFAULT);

					// �ǂݍ��ݎ��s������
					if (fontData.handle == -1) return;

					continue;
				}
				// ���V�[���Ŏg�p����ꍇ
				else if (fontData.scene != scene && !fontData.isNoEnd)
				{
					// �f�[�^�������Ă����ꍇ
					if (fontData.handle != -1)
					{
						// �������
						DeleteFontToHandle(fontData.handle);
						fontData.handle = -1;

						continue;
					}
				}
			}
		}

		/// <summary>
		/// �摜�n���h����n��
		/// </summary>
		/// <param name="id">ID            </param>
		/// <returns        >�n���h���f�[�^</returns>
		int GetHandle(const U& id)
		{
			// �}�b�v����ID�ɑΉ�����t�H���g�f�[�^������
			auto it = m_font.find(id);

			// ID������������n���h����Ԃ��A������Ȃ������� -1 ��Ԃ�
			if (it != m_font.end())
			{
				return it->second.handle;
			}
		}

	private:
		// �t�H���g�Ǘ��f�[�^
		std::map<T, FontData> m_font;

		// �t�H���g������t�H���_�[
		std::string m_folderPath;

	};
}