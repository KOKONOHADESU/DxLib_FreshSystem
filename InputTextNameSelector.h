#pragma once

#include <vector>
#include <string>

/// <summary>
/// ������I������
/// </summary>
class InputTextNameSelector
{
public:
	InputTextNameSelector();
	~InputTextNameSelector();

	// �X�V
	void Update();
	// �`��
	void Draw();

private:
	std::vector<std::string> m_textSelect;
};

