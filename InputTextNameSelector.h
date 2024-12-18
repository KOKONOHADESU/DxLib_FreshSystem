#pragma once

#include <vector>
#include <string>

/// <summary>
/// 文字を選択する
/// </summary>
class InputTextNameSelector
{
public:
	InputTextNameSelector();
	~InputTextNameSelector();

	// 更新
	void Update();
	// 描画
	void Draw();

private:
	std::vector<std::string> m_textSelect;
};

