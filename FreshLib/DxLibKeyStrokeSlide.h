#pragma once

#include "DxLibKeyFresh.h"

#include <DxLib.h>

namespace
{
	// �X���C�h����܂ł̃{�^���������Ă���t���[��
	constexpr int kSelectFrameMax = 30;
	// �X���C�h����܂ł̃{�^���������Ă���ŏ��t���[��
	constexpr int kSelectFrameMin = 1;
}

/// <summary>
/// �L�[�{�[�h�֘A
/// </summary>
namespace KEY
{
	class DxLibKeyStrokeSlide final
	{
	public:

		/// <summary>
		/// �R���X�g���N�^�œ��̓X���C�h�̐ݒ������
		/// </summary>
		/// <param name="selectMaxNum"   >�I���ł���ő吔                    </param>
		/// <param name="keyStrokeDirOne">�X���C�h���邽�߂̃L�[��ݒ�}�C�i�X</param>
		/// <param name="keyStrokeDirTow">�X���C�h���邽�߂̃L�[��ݒ�v���X  </param>
		DxLibKeyStrokeSlide(const int selectMaxNum, const int keyStrokeDirOne, const int keyStrokeDirTow , const int keyCheckSelect):
			m_selectNo(0),
			m_selectNoResult(-1),
			m_selectMaxNum(selectMaxNum),
			m_selectFramesPressCount(),
			m_selectUpSpeedFramePressCount(),
			m_isSelect(-1),
			m_selectFrameMax(),
			m_isKeyUp(false),
			m_isKeyDown(false),
			m_keySelect(),
			m_keyCheckSelect(keyCheckSelect)
		{
			// �X���C�h�p�L�[
			m_keySelect[0] = keyStrokeDirOne;
			m_keySelect[1] = keyStrokeDirTow;
		};
		~DxLibKeyStrokeSlide() {};

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update()
		{
			// �I��҂�
			KeyStrokeDirOneUpdate();
			KeyStrokeDirTwoUpdate();
			// �I������
			CheckSelect();
		}

		/// <summary>
		/// �I�����ʂ̊m�F
		/// </summary>
		/// <returns>�I�������l</returns>
		int GetResult() const
		{
			return m_selectNoResult;
		}

		/// <summary>
		/// ���ݑI�𒆂̔ԍ�
		/// </summary>
		/// <returns>�I�𒆂̔ԍ�</returns>
		int GetSelect() const
		{
			return m_selectNo;
		}

		/// <summary>
		/// �}�C�i�X�̒l����͂��Ă��邩�ǂ���
		/// </summary>
		/// <returns>�}�C�i�X�̒l�̓��͂����Ă��邩�ǂ���</returns>
		bool IsUpBotton() const
		{
			return m_isKeyUp;
		}

		/// <summary>
		/// �v���X�̒l����͂��Ă��邩�ǂ���
		/// </summary>
		/// <returns>�v���X�̒l�̓��͂����Ă��邩�ǂ���</returns>
		bool IsDownBotton() const
		{
			return m_isKeyDown;
		}

		/// <summary>
		/// �I�𒆂̈ʒu�����Z�b�g���邩�ǂ���
		/// </summary>
		void Reset() 
		{
			m_selectNo = 0;
		}

	private:
		/// <summary>
		/// �}�C�i�X�̒l�œ��͂��m�F����
		/// </summary>
		void KeyStrokeDirOneUpdate()
		{
			m_isKeyUp = false;
			// ��ɃX���C�h		
			if (uDxKey::GetInstance()->IsPress(m_keySelect[0]))
			{
				m_isKeyUp = true;

				// �{�^���������Ă���ԃt���[���P�ʂŃJ�E���g����
				m_selectFramesPressCount[SelectButton::UP]++;
				m_selectUpSpeedFramePressCount[SelectButton::UP]++;

				// �I���{�^�����������u�Ԃ�
				// �I���{�^�������������Ă���ۂɑI����ύX����
				if (m_selectFramesPressCount[SelectButton::UP] == 1 ||
					m_selectUpSpeedFramePressCount[SelectButton::UP] > m_selectFrameMax[SelectButton::UP])
				{
					// �I����ύX����
					m_selectNo--;

					// �I�������ꍇ�t���[��������������
					m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;

					if (m_selectFrameMax[SelectButton::UP] > kSelectFrameMin)
					{
						m_selectFrameMax[SelectButton::UP]--;
					}

					// �ŏ����ɂȂ�ƍő吔�ɕύX����
					if (m_selectNo <= -1)
					{
						m_selectNo = m_selectMaxNum;
					}
				}
			}
			else
			{
				// �t���[���J�E���g������������
				m_selectFramesPressCount[SelectButton::UP] = 0;
				m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;

				m_selectFrameMax[SelectButton::UP] = kSelectFrameMax;
			}
		}

		/// <summary>
		/// �v���X�̒l�œ��͂��m�F����
		/// </summary>
		void KeyStrokeDirTwoUpdate()
		{
			m_isKeyDown = false;
			// ���ɃX���C�h
			if (uDxKey::GetInstance()->IsPress(m_keySelect[1]))
			{
				m_isKeyDown = true;

				// �{�^���������Ă���ԃt���[���P�ʂŃJ�E���g����
				m_selectFramesPressCount[SelectButton::DOWN]++;
				m_selectUpSpeedFramePressCount[SelectButton::DOWN]++;

				// �I���{�^�����������u�Ԃ�
				// �I���{�^�������������Ă���ۂɑI����ύX����
				if (m_selectFramesPressCount[SelectButton::DOWN] == 1 ||
					m_selectUpSpeedFramePressCount[SelectButton::DOWN] > m_selectFrameMax[SelectButton::DOWN])
				{
					// �I����ύX����
					m_selectNo++;

					// �I�������ꍇ�t���[��������������
					m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;

					if (m_selectFrameMax[SelectButton::DOWN] > kSelectFrameMin)
					{
						m_selectFrameMax[SelectButton::DOWN]--;
					}

					// �ő吔�ɂȂ�ƍŏ����ɂ���
					if (m_selectNo > m_selectMaxNum)
					{
						m_selectNo = 0;
					}
				}
			}
			else
			{
				// �t���[���J�E���g������������
				m_selectFramesPressCount[SelectButton::DOWN] = 0;
				m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;

				m_selectFrameMax[SelectButton::DOWN] = kSelectFrameMax;
			}
		}

		/// <summary>
		/// ���͌�����m�F����
		/// </summary>
		void CheckSelect()
		{
			// �I��
			if (uDxKey::GetInstance()->IsTrigger(m_keyCheckSelect))
			{
				m_selectNoResult = m_selectNo;
			}
		}

	private:
		// �{�^���̎��
		enum SelectButton
		{
			UP,
			DOWN,

			BUTTONMAX,
		};
	private:

		// �L�[���͗p
		using uDxKey = KEY::DxLibKeyFresh;

		// �I�𒆂̔ԍ�
		int m_selectNo;

		// �I������
		int m_selectNoResult;

		// �I���ł���ő�̐�
		int m_selectMaxNum;

		// �I��p�t���[���J�E���g
		int m_selectFramesPressCount[SelectButton::BUTTONMAX];
		int m_selectUpSpeedFramePressCount[SelectButton::BUTTONMAX];

		// �I��ύX���\���ǂ���
		int m_isSelect;

		// �I����ύX����ׂ̃t���[��
		int m_selectFrameMax[SelectButton::BUTTONMAX];

		// ��{�^���������Ă��邩�ǂ���
		bool m_isKeyUp;
		// ���{�^���������Ă��邩�ǂ���
		bool m_isKeyDown;

		// �㉺�A���E�ǂ���̃{�^���g�p���邩�����߂�
		int m_keySelect[2];

		// ������L�[
		int m_keyCheckSelect;
	};
}
