#pragma once

#include "Text.h"

namespace LindaEditor
{
	class TextWrapped : public Text
	{
	public:
		TextWrapped(const std::string& p_content = "");

	protected:
		virtual void _Draw_Impl() override;
	};
}
