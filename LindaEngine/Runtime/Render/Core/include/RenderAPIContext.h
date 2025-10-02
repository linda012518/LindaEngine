#pragma once

#include "AutoPtr.h"
#include "GraphicsDriverAPI.h"

namespace LindaEngine
{
	class RenderAPIContext
	{
	public:
		virtual void SetViewport(int xStart, int yStart, int width, int height) = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear(bool color, bool depth, bool stencil) = 0;

		static Ref<RenderAPIContext> Create(GraphicsDriverAPI api);
	};
}
