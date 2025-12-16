#pragma once

#include "Widget.h"
#include "TextureID.h"

#include "glm/glm.hpp"

namespace LindaEditor
{
	class Image : public Widget
	{
	public:
		Image(uint32_t p_textureID, const glm::vec2& p_size);

	protected:
		void _Draw_Impl() override;

	public:
		TextureID textureID;
		glm::vec2 size;
	};
}
