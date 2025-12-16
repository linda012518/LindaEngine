#pragma once

#include "Button.h"

namespace LindaEditor
{
	class ButtonColored : public Button
	{
	public:
		ButtonColored(const std::string& p_label = "", const glm::vec4& p_color = {}, const glm::vec2& p_size = glm::vec2(0.f, 0.f), bool p_enableAlpha = true);

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;
		glm::vec4 color;
		glm::vec2 size;
		bool enableAlpha;
	};
}
