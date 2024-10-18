#pragma once

#include "SingletonBase.h"

#include <DxLib.h>

namespace
{
	// �L�[�o�b�t�@
	constexpr int kKeyStateBuf = 256;

	// �g���K�[�p�t���[���J�E���g
	constexpr int kKeyTriggerFrameCount = 1;
}

/// <summary>
/// �p�b�h�֘A
/// </summary>
namespace Key
{
	/// <summary>
	/// �p�b�h����
	/// </summary>
	class DxLibKeyFresh : public SingletonBase<DxLibKeyFresh>
	{
	public:
		// SingletonBase�N���X�ł̂݃C���X�^���X������������
		friend class SingletonBase<DxLibKeyFresh>;

		~DxLibKeyFresh() {};

	private:
		DxLibKeyFresh() :
			m_keyBuf(),
			m_isTrigger(false),
			m_pressFrameCount(0)
		{

		}

	public:
		/// <summary>
		/// �p�b�h�̓��͏�Ԏ擾
		/// </summary>
		void Update()
		{
			// �L�[�{�[�h�̓��͏�Ԃ��擾����
			GetHitKeyStateAll(m_keyBuf);
		}

		/// <summary>
		/// ������������
		/// </summary>
		/// <param name="keyNo">���͂���L�[                            </param>
		/// <returns           >true : �����Ă��� , false : �����Ă��Ȃ�</returns>
		bool IsPress(int keyNo) const
		{
			// �w��̃L�[���͂̔��������
			if (m_keyBuf[keyNo])
			{
				return true;
			}
			return false;
		}

		/// <summary>
		/// �g���K�[����
		/// </summary>
		/// <param name="keyNo">���͂���L�[                            </param>
		/// <returns           >true : �����Ă��� , false : �����Ă��Ȃ�</returns>
		bool IsTrigger(int keyNo) 
		{
			m_isTrigger = false;
			// �w��̃L�[���͂̔��������
			if (m_keyBuf[keyNo])
			{
				// �t���[���J�E���g
				m_pressFrameCount++;

				// �w�肵���t���[���̎��ɏ���������
				if (m_pressFrameCount == kKeyTriggerFrameCount)
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

	private:
		// �L�[�p�o�b�t�@
		char m_keyBuf[kKeyStateBuf]{};

		// �g���K�[�p��������
		bool m_isTrigger = false;

		// �����Ă���Ԃ̃t���[���J�E���g
		int m_pressFrameCount = 0;
	};
}