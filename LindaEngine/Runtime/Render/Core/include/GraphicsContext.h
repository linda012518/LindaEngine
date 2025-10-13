#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.hpp"
#include "GraphicsConfig.h"

namespace LindaEngine
{
	class RenderPipeline;
	class GraphicsContext : public IRuntimeModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;

		static GraphicsConfig graphicsConfig;

		static Scope<GraphicsContext> Create(void* window);

	protected:
		virtual void CreateRenderPipeline() = 0;

	protected:
		Scope<RenderPipeline> _renderPipeline;
	};
}
