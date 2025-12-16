#pragma once

#include "Button.h"
#include "TextureID.h"

namespace LindaEditor
{
	class ButtonImage : public Button
	{
	public:
		ButtonImage(uint32_t p_textureID, const glm::vec2& p_size);

	protected:
		void _Draw_Impl() override;

	public:
		bool disabled = false;

		glm::vec4 background = { 0, 0, 0, 0 };
		glm::vec4 tint = { 1, 1, 1, 1 };

		TextureID textureID;
		glm::vec2 size;
	};
}
