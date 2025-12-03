#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.h"
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
		virtual void SetRenderPipeline(Ref<RenderPipeline> pipeline) = 0;

		static GraphicsConfig graphicsConfig;

		static Scope<GraphicsContext> Create(void* window);

	protected:
		Ref<RenderPipeline> _renderPipeline;
	};
}
