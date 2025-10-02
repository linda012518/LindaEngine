#pragma once

#include "AutoPtr.h"
#include "Platform.h"
#include "IRuntimeModule.hpp"

namespace LindaEngine
{
	struct GfxConfiguration;
	class Window : public IRuntimeModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;

		static Scope<Window> Create(Platform platform, GfxConfiguration& gfx);
	};
}
