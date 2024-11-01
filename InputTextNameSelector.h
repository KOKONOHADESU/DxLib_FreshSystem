#pragma once

#include <vector>
#include <string>

/// <summary>
/// •¶š‚ğ‘I‘ğ‚·‚é
/// </summary>
class InputTextNameSelector
{
public:
	InputTextNameSelector();
	~InputTextNameSelector();

	// XV
	void Update();
	// •`‰æ
	void Draw();

private:
	std::vector<std::string> m_textSelect;
};

