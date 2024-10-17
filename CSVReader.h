#pragma once

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

#include <DxLib.h>

namespace
{
	// �g���q
	const std::string kExtension = ".csv";
}

/// <summary>
/// CSV�֘A
/// </summary>
namespace CSV
{	
	/// <summary>
	/// CSV�t�@�C���ǂݍ��ݗp
	/// </summary>
	/// <typeparam name="T">ID�p�̌^���w��</typeparam>
	template <class T>
	class CSVReader final
	{
	public:
		/// <summary>
		/// CSV�t�@�C�������݂���p�X���w��
		/// </summary>
		/// <param name="filePath"></param>		
		CSVReader(const std::string& filePath)
		{
			m_filePath = filePath;
		};

		~CSVReader()
		{
			
		};

		/// <summary>
		/// CSV�t�@�C���̓ǂݍ���
		/// </summary>
		/// <param name="id"           >ID���w�肷��              </param>
		/// <param name="fileName"     >�t�@�C���̖��O���w��      </param>
		/// <param name="ignoreCellNum">��������Z���̐�          </param>
		/// <returns                   >true : ���� , false : ���s</returns>
		bool Load(const T& id, const std::string& fileName, const int ignoreCellNum)
		{
			// �t�@�C������1�������ǂݍ��ޗp
			std::string line;

			std::string	filePath = m_filePath + fileName + kExtension;

			// �t�@�C���ǂݍ��ݗp
			std::ifstream ifs(filePath.c_str());

			// �t�@�C�����J���Ȃ��ꍇ
			if (!ifs)
			{
				return false;
			}

			// ��������Z�����J�E���g����
			int ignoreCellCount = 0;

			// �ǂݍ��݊J�n
			while (getline(ifs, line))
			{
				// �ǂݍ��܂Ȃ��Z�����΂�
				ignoreCellCount++;
				if (ignoreCellCount == ignoreCellNum) continue;

				// �ǂݍ��񂾍s���J���}�ŕ�������
				std::vector<std::string> strvec = Split(line, ',');
				
				m_readData[id].push_back(strvec);
			}

			// �t�@�C�������
			ifs.close();

			return true;
		}
		
		/// <summary>
		/// �ǂݍ��񂾃f�[�^�̈ꕔ��ǂݍ���
		/// </summary>
		/// <typeparam name="U"       >ID���w��    <</typeparam>
		/// <param name="loadDataType">�w�肵���^  </param>
		/// <param name="columns"     >����w��(��)</param>
		/// <param name="row"         >�s���w��(��)</param>
		/// <returns                  >�w�肵���Z���̗v�f���w�肵���^�ŕԂ�/returns>
		template <typename U>
		U StringData(const T& id, const int columns, const int row)
		{
			// �w�肵����������擾
			std::stringstream cellStream(m_readData[id][columns][row]);

			// �w�肵���^�̕ϐ����쐬
			U value{};

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
		// �t�@�C���p�X
		std::string m_filePath;

		// �ǂݍ��񂾃f�[�^
		std::map<T,std::vector<std::vector<std::string>>> m_readData;
	};
}

