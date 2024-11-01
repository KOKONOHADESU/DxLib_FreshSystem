#include "InputTextNameSelector.h"

InputTextNameSelector::InputTextNameSelector()
{
	for (const char* i = "В†"; i < "Вс"; i++)
	{
		m_textSelect.push_back(i);
	}
}

InputTextNameSelector::~InputTextNameSelector()
{
}

void InputTextNameSelector::Update()
{
}

void InputTextNameSelector::Draw()
{
}
