#pragma once

#include "SingletonBase.h"
#include "Vec2.h"

#include <DxLib.h>

namespace
{
	// �g���K�[�p�t���[���J�E���g
	constexpr int kMouseTriggerFrameCount = 1;
}

/// <summary>
/// �}�E�X�֘A
/// </summary>
namespace Mouse
{
	/// <summary>
	/// �p�b�h����
	/// </summary>
	class DxLibMouseFresh : public SingletonBase<DxLibMouseFresh>
	{
	public:
		// SingletonBase�N���X�ł̂݃C���X�^���X������������
		friend class SingletonBase<DxLibMouseFresh>;

		~DxLibMouseFresh() {};

	private:
		DxLibMouseFresh() :
			m_pos({ 0,0 }),
			m_isTrigger(false),
			m_pressFrameCount(0)
		{

		}

	public:
		/// <summary>
		/// �}�E�X�̏�Ԏ擾
		/// </summary>
		void Update()
		{
			int x = 0, y = 0;

			// �}�E�X�̍��W�X�V
			GetMousePoint(&x, &y);

			// ���W���R�s�[
			m_pos.x = static_cast<int>(x);
			m_pos.y = static_cast<int>(y);
		}

		/// <summary>
		/// ������������
		/// </summary>
		/// <param name="keyNo">���͂���L�[                            </param>
		/// <returns           >true : �����Ă��� , false : �����Ă��Ȃ�</returns>
		bool IsPress(int mouseNo) const
		{
			// ���͂��Ă��邩�ǂ���
			if ((GetMouseInput() & mouseNo))
			{
				return true;
			}
			else
			{
				return false;
			}			
		}

		/// <summary>
		/// �g���K�[����
		/// </summary>
		/// <param name="keyNo">���͂���L�[                            </param>
		/// <returns           >true : �����Ă��� , false : �����Ă��Ȃ�</returns>
		bool IsTrigger(int mouseNo)
		{
			m_isTrigger = false;

			// �w��̃L�[���͂̔��������
			if ((GetMouseInput() & mouseNo))
			{
				// �t���[���J�E���g
				m_pressFrameCount++;

				// �w�肵���t���[���̎��ɏ���������
				if (m_pressFrameCount == kMouseTriggerFrameCount)
				{
					m_isTrigger = true;
				}
			}
			else
			{
				// �����Ă��Ȃ��ꍇ�̓t���[���J�E���g�����Z�b�g
				m_pressFrameCount = 0;
			}

			return m_isTrigger;
		}
		
		/// <summary>
		/// �}�E�X�̍��W��n��
		/// </summary>
		/// <typeparam name="T">���W�̌^�����߂�</typeparam>
		/// <returns           >�}�E�X�̍��W    </returns>
		template <typename T>
		Vec2<T> GetPos()
		{
			// �L���X�g����
			Vec2<T> pos{ static_cast<T>(m_pos.x),static_cast<T>(m_pos.y) };

			// T�^�ɃL���X�g�������W��n��
			return pos;
		}

	private:
		// ���W
		Vec2<float> m_pos;

		// �g���K�[�p��������
		bool m_isTrigger = false;

		// �����Ă���Ԃ̃t���[���J�E���g
		int m_pressFrameCount = 0;
	};
}