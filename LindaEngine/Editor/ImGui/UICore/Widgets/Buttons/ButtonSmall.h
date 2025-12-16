#pragma once

#include "Button.h"

namespace LindaEditor
{
	class ButtonSmall : public Button
	{
	public:
		ButtonSmall(const std::string& p_label = "");

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;

		glm::vec4 idleBackgroundColor;
		glm::vec4 hoveredBackgroundColor;
		glm::vec4 clickedBackgroundColor;

		glm::vec4 textColor;

	};
}
