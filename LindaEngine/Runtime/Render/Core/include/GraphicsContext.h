#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.hpp"
#include "GraphicsDriverAPI.h"
#include "GfxConfiguration.h"

namespace LindaEngine
{
	class GraphicsContext : public IRuntimeModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;

		static Scope<GraphicsContext> Create(GraphicsDriverAPI api, void* window);
	};
}
