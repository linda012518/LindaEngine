#pragma once

#include "Widget.h"

#include "glm/glm.hpp"

namespace LindaEditor
{
	class ProgressBar : public Widget
	{
	public:
		ProgressBar(float p_fraction = 0.0f, const glm::vec2& p_size = { 0.0f, 0.0f }, const std::string& p_overlay = "");

	protected:
		void _Draw_Impl() override;

	public:
		float fraction;
		glm::vec2 size;
		std::string overlay;
	};
}
