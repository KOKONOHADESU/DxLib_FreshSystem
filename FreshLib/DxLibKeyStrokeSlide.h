#pragma once

#include "DxLibKeyFresh.h"

#include <DxLib.h>

namespace
{
	// スライドするまでのボタンを押しているフレーム
	constexpr int kSelectFrameMax = 30;
	// スライドするまでのボタンを押している最小フレーム
	constexpr int kSelectFrameMin = 1;
}

/// <summary>
/// キーボード関連
/// </summary>
namespace KEY
{
	class DxLibKeyStrokeSlide final
	{
	public:

		/// <summary>
		/// コンストラクタで入力スライドの設定をする
		/// </summary>
		/// <param name="selectMaxNum"   >選択できる最大数                    </param>
		/// <param name="keyStrokeDirOne">スライドするためのキーを設定マイナス</param>
		/// <param name="keyStrokeDirTow">スライドするためのキーを設定プラス  </param>
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
			// スライド用キー
			m_keySelect[0] = keyStrokeDirOne;
			m_keySelect[1] = keyStrokeDirTow;
		};
		~DxLibKeyStrokeSlide() {};

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update()
		{
			// 選択待ち
			KeyStrokeDirOneUpdate();
			KeyStrokeDirTwoUpdate();
			// 選択決定
			CheckSelect();
		}

		/// <summary>
		/// 選択結果の確認
		/// </summary>
		/// <returns>選択した値</returns>
		int GetResult() const
		{
			return m_selectNoResult;
		}

		/// <summary>
		/// 現在選択中の番号
		/// </summary>
		/// <returns>選択中の番号</returns>
		int GetSelect() const
		{
			return m_selectNo;
		}

		/// <summary>
		/// マイナスの値を入力しているかどうか
		/// </summary>
		/// <returns>マイナスの値の入力をしているかどうか</returns>
		bool IsUpBotton() const
		{
			return m_isKeyUp;
		}

		/// <summary>
		/// プラスの値を入力しているかどうか
		/// </summary>
		/// <returns>プラスの値の入力をしているかどうか</returns>
		bool IsDownBotton() const
		{
			return m_isKeyDown;
		}

		/// <summary>
		/// 選択中の位置をリセットするかどうか
		/// </summary>
		void Reset() 
		{
			m_selectNo = 0;
		}

	private:
		/// <summary>
		/// マイナスの値で入力を確認する
		/// </summary>
		void KeyStrokeDirOneUpdate()
		{
			m_isKeyUp = false;
			// 上にスライド		
			if (uDxKey::GetInstance()->IsPress(m_keySelect[0]))
			{
				m_isKeyUp = true;

				// ボタンを押している間フレーム単位でカウントする
				m_selectFramesPressCount[SelectButton::UP]++;
				m_selectUpSpeedFramePressCount[SelectButton::UP]++;

				// 選択ボタンを押した瞬間か
				// 選択ボタンを押し続けている際に選択を変更する
				if (m_selectFramesPressCount[SelectButton::UP] == 1 ||
					m_selectUpSpeedFramePressCount[SelectButton::UP] > m_selectFrameMax[SelectButton::UP])
				{
					// 選択を変更する
					m_selectNo--;

					// 選択した場合フレームを初期化する
					m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;

					if (m_selectFrameMax[SelectButton::UP] > kSelectFrameMin)
					{
						m_selectFrameMax[SelectButton::UP]--;
					}

					// 最小数になると最大数に変更する
					if (m_selectNo <= -1)
					{
						m_selectNo = m_selectMaxNum;
					}
				}
			}
			else
			{
				// フレームカウントを初期化する
				m_selectFramesPressCount[SelectButton::UP] = 0;
				m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;

				m_selectFrameMax[SelectButton::UP] = kSelectFrameMax;
			}
		}

		/// <summary>
		/// プラスの値で入力を確認する
		/// </summary>
		void KeyStrokeDirTwoUpdate()
		{
			m_isKeyDown = false;
			// 下にスライド
			if (uDxKey::GetInstance()->IsPress(m_keySelect[1]))
			{
				m_isKeyDown = true;

				// ボタンを押している間フレーム単位でカウントする
				m_selectFramesPressCount[SelectButton::DOWN]++;
				m_selectUpSpeedFramePressCount[SelectButton::DOWN]++;

				// 選択ボタンを押した瞬間か
				// 選択ボタンを押し続けている際に選択を変更する
				if (m_selectFramesPressCount[SelectButton::DOWN] == 1 ||
					m_selectUpSpeedFramePressCount[SelectButton::DOWN] > m_selectFrameMax[SelectButton::DOWN])
				{
					// 選択を変更する
					m_selectNo++;

					// 選択した場合フレームを初期化する
					m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;

					if (m_selectFrameMax[SelectButton::DOWN] > kSelectFrameMin)
					{
						m_selectFrameMax[SelectButton::DOWN]--;
					}

					// 最大数になると最小数にする
					if (m_selectNo > m_selectMaxNum)
					{
						m_selectNo = 0;
					}
				}
			}
			else
			{
				// フレームカウントを初期化する
				m_selectFramesPressCount[SelectButton::DOWN] = 0;
				m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;

				m_selectFrameMax[SelectButton::DOWN] = kSelectFrameMax;
			}
		}

		/// <summary>
		/// 入力決定を確認する
		/// </summary>
		void CheckSelect()
		{
			// 選択
			if (uDxKey::GetInstance()->IsTrigger(m_keyCheckSelect))
			{
				m_selectNoResult = m_selectNo;
			}
		}

	private:
		// ボタンの種類
		enum SelectButton
		{
			UP,
			DOWN,

			BUTTONMAX,
		};
	private:

		// キー入力用
		using uDxKey = KEY::DxLibKeyFresh;

		// 選択中の番号
		int m_selectNo;

		// 選択結果
		int m_selectNoResult;

		// 選択できる最大の数
		int m_selectMaxNum;

		// 選択用フレームカウント
		int m_selectFramesPressCount[SelectButton::BUTTONMAX];
		int m_selectUpSpeedFramePressCount[SelectButton::BUTTONMAX];

		// 選択変更が可能かどうか
		int m_isSelect;

		// 選択を変更する為のフレーム
		int m_selectFrameMax[SelectButton::BUTTONMAX];

		// 上ボタンを押しているかどうか
		bool m_isKeyUp;
		// 下ボタンを押しているかどうか
		bool m_isKeyDown;

		// 上下、左右どちらのボタン使用するかを決める
		int m_keySelect[2];

		// 決定をキー
		int m_keyCheckSelect;
	};
}
