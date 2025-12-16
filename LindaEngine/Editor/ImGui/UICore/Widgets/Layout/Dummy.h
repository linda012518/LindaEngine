#pragma once

#include "Widget.h"

#include "glm/glm.hpp"

namespace LindaEditor
{
	class Dummy : public Widget
	{
	public:
		Dummy(const glm::vec2& p_size = { 0.0f, 0.0f });

	protected:
		void _Draw_Impl() override;

	public:
		glm::vec2 size;
	};
}
