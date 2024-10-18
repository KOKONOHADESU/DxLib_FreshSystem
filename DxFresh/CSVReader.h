#pragma once

#include "SingletonBase.h"
#include "CSVCommon.h"

#include <vector>
#include <map>
#include <sstream>
#include <fstream>

/// <summary>
/// CSV�֘A
/// </summary>
namespace CSV
{	
	/// <summary>
	/// CSV�t�@�C���ǂݍ��ݗp
	/// </summary>
	/// <typeparam name="T">ID�p�̌^���w��</typeparam>
	/// <typeparam name="U">�ǂ̃V�[���ŉ摜��ǂݍ��ނ������Ă��邷��^</typeparam>
	template <typename T, typename U>
	class CSVReader : public SingletonBase<CSVReader<T, U>>
	{
	private:
		/// <summary>
		/// �ǂݍ��ݗp�f�[�^
		/// </summary>
		struct readerData
		{
			T id;                                             // ID
			U scene;                                          // �V�[��	
			std::string filePath;                             // �p�X
			std::vector<std::vector<std::string>> stringData; // CSV����ǂݍ��񂾕���
			int ignoreCellNum;                                // ��������Z��
			bool isNoEnd;                                     // �ǂ̃V�[���ł��g�p����ꍇ
		};
	public:
		// SingletonBase�N���X�̃A�N�Z�X��������
		friend class SingletonBase<CSVReader<T, U>>;

	private:
		CSVReader(){};

	public:
		~CSVReader(){};

		/// <summary>
		/// �摜������K�w�̃t�H���_�[���w��
		/// </summary>
		/// <param name="grahicFolderPath"></param>
		void CSVFolderPath(const char* grahicFolderPath)
		{
			m_folderPath = grahicFolderPath;
		}

		/// <summary>
		/// CSV�t�@�C���̓ǂݍ���
		/// </summary>
		/// <param name="id"           >ID���w�肷��                </param>
		/// <param name="scene"        >�g�p����V�[���̌^          </param>
		/// <param name="fileName"     >�t�@�C���̖��O���w��        </param>
		/// <param name="ignoreCellNum">��������Z���̐�            </param>
		/// <param name="isNoEnd"      >���̃V�[���Ŏg�p���邩�ǂ���</param>
		void Add(const T& id, const U& scene, const char* fileName, const int ignoreCellNum, bool isNoEnd = false)
		{
			readerData data{};

			// ID
			data.id = id;

			// �g�p����V�[��
			data.scene = scene;

			// �t�@�C���p�X
			data.filePath = m_folderPath + fileName + kExtension;

			// ���ׂẴV�[���Ŏg�p���邩�ǂ���
			if (isNoEnd)
			{				
				// �ǂݍ���
				data.stringData = Read(data.filePath.c_str(), ignoreCellNum);
			}
			else
			{
				// �ǂݍ��܂Ȃ�
				data.stringData = { {""} };
			}

			// ��������Z��
			data.ignoreCellNum = ignoreCellNum;

			// ���ׂẴV�[���Ŏg�p���邩�ǂ���
			data.isNoEnd = isNoEnd;			

			// �f�[�^�̒ǉ�
			m_readData.push_back(data);
		}

		/// <summary>
		/// ���݂̃V�[�����m�F����
		/// </summary>
		/// <param name="scene">���݂̃V�[��</param>
		void SceneInput(const U& scene)
		{
			// ���ׂẴf�[�^���m�F
			for (int i = 0; i < m_readData.size(); i++)
			{				
				// ���݂̃V�[���Ŏg�p���邩�ǂ���
				if (m_readData[i].scene == scene)
				{
					// ���[�h
					m_readData[i].stringData = Read(m_readData[i].filePath.c_str(), m_readData[i].ignoreCellNum);

					continue;
				}
				// ���V�[���Ŏg�p����ꍇ
				else if (m_readData[i].scene != scene && !m_readData[i].isNoEnd)
				{									
					m_readData[i].stringData = { {""} };

					continue;				
				}
			}
		}

		/// <summary>
		/// CSV�t�@�C����ǂݍ���œn��
		/// </summary>
		/// <param name="fileName"     >�t�@�C����             </param>
		/// <param name="ignoreCellNum">��������Z��           </param>
		/// <returns                   >CSV����ǂݍ��񂾕�����</returns>
		std::vector<std::vector<std::string>> Read(const char* fileName, int ignoreCellNum)
		{
			// �t�@�C������1�������ǂݍ��ޗp
			std::string line;

			// �t�@�C���ǂݍ��ݗp
			std::ifstream ifs(fileName);

			// �t�@�C�����J���Ȃ��ꍇ
			if (!ifs)
			{
				
			}

			// ��������Z�����J�E���g����
			int ignoreCellCount = 0;

			std::vector<std::vector<std::string>> string;

			// �ǂݍ��݊J�n
			while (getline(ifs, line))
			{
				// �ǂݍ��܂Ȃ��Z�����΂�
				ignoreCellCount++;
				if (ignoreCellCount == ignoreCellNum) continue;

				// �ǂݍ��񂾍s���J���}�ŕ�������
				std::vector<std::string> strvec = Split(line, ',');

				// �f�[�^���L�^
				string.push_back(strvec);
			}

			// �t�@�C�������
			ifs.close();

			return string;
		}
		
		/// <summary>
		/// �ǂݍ��񂾃f�[�^�̈ꕔ��ǂݍ���
		/// </summary>
		/// <typeparam name="U"       >ID���w��    <</typeparam>
		/// <param name="loadDataType">�w�肵���^  </param>
		/// <param name="columns"     >����w��(��)</param>
		/// <param name="row"         >�s���w��(��)</param>
		/// <returns                  >�w�肵���Z���̗v�f���w�肵���^�ŕԂ�/returns>
		template <typename V>
		V StringData(const T& id, const int columns, const int row)
		{
			std::stringstream cellStream{};

			// ���ׂĂ̊m�F
			for (int i = 0; i < m_readData.size(); i++)
			{
				// �g�p����V�[�����m�F
				if (m_readData[i].id == id)
				{
					// �w��̃Z�����當������m�F
					std::string data = m_readData[i].stringData[row][columns];
					// �X�g���[���Ƀf�[�^��}��
					cellStream << data; 

					break;
				}
			}

			// �w�肵���^�̕ϐ����쐬
			V value{};

			// �X�g���[�����Z�q���g���w�肵���^�ɕϊ�����
			cellStream >> value;

			// �l��Ԃ�
			return value;
		}



	private:
		/// <summary>
		/// CSV�؂蔲���p
		/// </summary>
		/// <param name="input"    >CSV�f�[�^�̕�����</param>
		/// <param name="delimiter">�؂蔲���ۂ̕��� </param>
		/// <returns></returns>
		std::vector<std::string> Split(const std::string& input, char delimiter) const
		{
			// ���͕�������X�g���[���ɕϊ�
			std::istringstream stream(input);

			// �������ꂽ�e�t�B�[���h���i�[���邽�߂̕ϐ�
			std::string field;

			// �������ꂽ��������i�[����x�N�^�[
			std::vector<std::string> result;

			// �X�g���[������1���t�B�[���h�����o���A�x�N�^�[�ɒǉ�
			while (getline(stream, field, delimiter))
			{
				result.push_back(field);
			}

			// �������ꂽ������̃x�N�^�[��Ԃ�
			return result;
		}

	private:
		// �t�H���_�p�X�L�^
		std::string m_folderPath;

		// �ǂݍ��񂾃f�[�^
		std::vector<readerData> m_readData;
	};
}

