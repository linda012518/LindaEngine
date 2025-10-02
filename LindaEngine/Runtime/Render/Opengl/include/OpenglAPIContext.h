#pragma once

#include "RenderAPIContext.h"

namespace LindaEngine
{
	class OpenglAPIContext : public RenderAPIContext
	{
	public:
		void SetViewport(int xStart, int yStart, int width, int height);
		void SetClearColor(float r, float g, float b, float a);
		void Clear(bool color, bool depth, bool stencil);
	};
}
