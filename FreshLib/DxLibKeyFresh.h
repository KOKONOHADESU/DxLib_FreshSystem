#pragma once

#include "SingletonBase.h"

#include <DxLib.h>

namespace
{
	// キーバッファ
	constexpr int kKeyStateBuf = 256;

	// トリガー用フレームカウント
	constexpr int kKeyTriggerFrameCount = 1;
}

/// <summary>
/// キーボード関連
/// </summary>
namespace KEY
{
	/// <summary>
	/// キーボード処理
	/// </summary>
	class DxLibKeyFresh : public SingletonBase<DxLibKeyFresh>
	{
	public:
		// SingletonBaseクラスでのみインスタンス生成を許可する
		friend class SingletonBase<DxLibKeyFresh>;

		~DxLibKeyFresh() {};

	private:
		DxLibKeyFresh() :
			m_keyBuf(),
			m_pressFrameCount(0)
		{

		}

	public:
		/// <summary>
		/// キーボードの入力状態取得
		/// </summary>
		void Update()
		{
			// キーボードの入力状態を取得する
			GetHitKeyStateAll(m_keyBuf);
		}

		/// <summary>
		/// 押し下げ判定
		/// </summary>
		/// <param name="keyNo">入力するキー                            </param>
		/// <returns           >true : 押している , false : 押していない</returns>
		bool IsPress(int keyNo) const
		{
			// 指定のキー入力の判定をする
			if (m_keyBuf[keyNo])
			{
				return true;
			}
			return false;
		}

		/// <summary>
		/// トリガー判定
		/// </summary>
		/// <param name="keyNo">入力するキー                            </param>
		/// <returns           >true : 押している , false : 押していない</returns>
		bool IsTrigger(int keyNo) 
		{
			// 指定のキー入力の判定をする
			if (m_keyBuf[keyNo])
			{
				// フレームカウント
				m_pressFrameCount++;

				// 指定したフレームの時に処理をする
				if (m_pressFrameCount == kKeyTriggerFrameCount)
				{
					return true;
				}
			}
			else
			{
				// 押していない場合はフレームカウントをリセット
				m_pressFrameCount = 0;
			}

			return false;
		}

	private:
		// キー用バッファ
		char m_keyBuf[kKeyStateBuf]{};

		// 押している間のフレームカウント
		int m_pressFrameCount = 0;
	};
}