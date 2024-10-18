#pragma once

#include "SingletonBase.h"

#include <random>

/// <summary>
/// 乱数関連
/// </summary>
namespace Rand
{
	// メルセンヌ・ツイスタ使用乱数
	class MTRandom : public SingletonBase<MTRandom>
	{
	public:
		// SingletonBaseクラスのアクセスを許可する
		friend class SingletonBase<MTRandom>;

		~MTRandom() {};
	private:
		MTRandom()
		{
			// 乱数生成器を初期化
			std::random_device rand;
			// メルセンヌツイスタで乱数生成
			m_randGenerator = std::mt19937(rand());
		}

	public:
		/// <summary>
		/// メルセンヌ・ツイスタを使用した乱数の生成 
		/// </summary>
		/// <param name="randMin">最小数                            </param>
		/// <param name="randMax">最大数                            </param>
		/// <returns             >引数で指定した数の中から乱数を渡す</returns>
		template <typename T>
		T GetMTRand(T randMin, T randMax)
		{
			// メルセンヌツイスタで乱数の範囲を設定
			std::uniform_int_distribution<> distrib(randMin, randMax);

			// 範囲でしたいした値をランダムで返す
			return static_cast<T>(distrib(m_randGenerator));
		}

	private:
		// 乱数生成器
		std::mt19937 m_randGenerator;
	};
}