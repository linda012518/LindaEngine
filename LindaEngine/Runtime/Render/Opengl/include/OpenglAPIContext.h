#pragma once

#include "RenderAPIContext.h"
#include "RenderState.h"

namespace LindaEngine
{
	class OpenglAPIContext : public RenderAPIContext
	{
	public:
		void SetViewport(int xStart, int yStart, int width, int height);
		void SetClearColor(float r, float g, float b, float a);
		void Clear(bool color, bool depth, bool stencil);

		void CheckRenderState(RenderState& state, RenderState& materialState);

	private:
		void CheckColorMask(RenderState& state, RenderState& materialState);
		void CheckDepthState(RenderState& state, RenderState& materialState);
		void CheckCullFaceState(RenderState& state, RenderState& materialState);
		void CheckPolygonModeState(RenderState& state, RenderState& materialState);
		void CheckStencilState(RenderState& state, RenderState& materialState);
		void CheckBlendState(RenderState& state, RenderState& materialState);
	};
}
