#pragma once

#include "SingletonBase.h"

#include <random>

/// <summary>
/// �����֘A
/// </summary>
namespace Rand
{
	// �����Z���k�E�c�C�X�^�g�p����
	class MTRandom : public SingletonBase<MTRandom>
	{
	public:
		// SingletonBase�N���X�̃A�N�Z�X��������
		friend class SingletonBase<MTRandom>;

		~MTRandom() {};
	private:
		MTRandom()
		{
			// �����������������
			std::random_device rand;
			// �����Z���k�c�C�X�^�ŗ�������
			m_randGenerator = std::mt19937(rand());
		}

	public:
		/// <summary>
		/// �����Z���k�E�c�C�X�^���g�p���������̐��� 
		/// </summary>
		/// <param name="randMin">�ŏ���                            </param>
		/// <param name="randMax">�ő吔                            </param>
		/// <returns             >�����Ŏw�肵�����̒����痐����n��</returns>
		template <typename T>
		T GetMTRand(T randMin, T randMax)
		{
			// �����Z���k�c�C�X�^�ŗ����͈̔͂�ݒ�
			std::uniform_int_distribution<> distrib(randMin, randMax);

			// �͈͂ł����������l�������_���ŕԂ�
			return static_cast<T>(distrib(m_randGenerator));
		}

	private:
		// ����������
		std::mt19937 m_randGenerator;
	};
}