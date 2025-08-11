#pragma once

#include "IRuntimeModule.hpp"

namespace LindaEngine
{
	Interface IApplication : implements IRuntimeModule
	{
	public:
		virtual int Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;

		virtual bool IsQuit() const = 0;
	};

	extern IApplication* g_pApp;
}
