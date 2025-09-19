#pragma once

#include "Interface/IRuntimeModule.hpp"

namespace LindaEngine
{
	class GraphicsContext : implements IRuntimeModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;
	};

	extern GraphicsContext* g_GraphicsContext;
}
