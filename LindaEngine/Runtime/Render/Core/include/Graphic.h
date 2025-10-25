#pragma once

#include "AutoPtr.h"

namespace LindaEngine
{
	struct RenderState;
	class RenderAPIContext;

	class Graphic
	{
	public:
		static void SetViewport(int xStart, int yStart, int width, int height);
		static void SetClearColor(float r, float g, float b, float a);
		static void Clear(bool color, bool depth, bool stencil);

		static void CheckRenderState(RenderState& state, RenderState& materialState);

	private:
		static Scope<RenderAPIContext> _renderContext;
	};
}
