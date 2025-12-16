#pragma once

#include "DataWidget.h"
#include "EventCallback.h"
#include "glm/glm.hpp"

namespace LindaEditor
{
	class ColorEdit : public DataWidget<glm::vec4>
	{
	public:
		ColorEdit(bool p_enableAlpha = false, const glm::vec4& p_defaultColor = {});

	protected:
		void _Draw_Impl() override;

	public:
		bool enableAlpha;
		glm::vec4 color;
		LindaEngine::EventCallback<glm::vec4&> ColorChangedEvent;
	};
}
