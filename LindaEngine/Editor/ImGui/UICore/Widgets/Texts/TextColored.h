#pragma once

#include "Text.h"

#include "glm/glm.hpp"

namespace LindaEditor
{
	class TextColored : public Text
	{
	public:
		TextColored(const std::string& p_content = "", const glm::vec4& p_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	public:
		glm::vec4 color;

	protected:
		virtual void _Draw_Impl() override;
	};
}
