#pragma once

#include "IRuntimeModule.hpp"

#include "Window.h"
#include "GraphicsContext.h"

namespace LindaEngine
{
	class Application : public IRuntimeModule
	{
	public:
		int Initialize();
		void Finalize();
		void Tick();

		static void Quit();

	private:
		static bool _isQuit;

		Scope<Window> _window;
		Scope<GraphicsContext> _graphicContext;
	};
}
