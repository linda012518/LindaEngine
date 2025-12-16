#pragma once

#include "Text.h"

namespace LindaEditor
{
	class TextDisabled : public Text
	{
	public:
		TextDisabled(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;
	};
}
