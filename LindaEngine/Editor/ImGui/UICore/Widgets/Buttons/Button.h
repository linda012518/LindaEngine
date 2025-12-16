#pragma once

#include "ButtonBase.h"
#include "glm/glm.hpp"

namespace LindaEditor
{
	class Button : public ButtonBase
	{
	public:
		Button(const std::string& p_label = "", const glm::vec2& p_size = glm::vec2(0.f, 0.f), bool p_disabled = false);

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;
		glm::vec2 size;
		bool disabled = false;

		glm::vec4 idleBackgroundColor;
		glm::vec4 hoveredBackgroundColor;
		glm::vec4 clickedBackgroundColor;

		glm::vec4 textColor;
	};
}
