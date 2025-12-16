#include "Panel.h"

using namespace LindaEditor;

uint64_t Panel::__PANEL_ID_INCREMENT = 0;

Panel::Panel()
{
	_panelID = "##" + std::to_string(__PANEL_ID_INCREMENT++);
}

void Panel::Draw()
{
	if (enabled)
	{
		_Draw_Impl();
	}
}

const std::string& Panel::GetPanelID() const
{
	return _panelID;
}
