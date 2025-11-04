#pragma once

#include "IRuntimeModule.h"

#include "Window.h"
#include "GraphicsContext.h"
#include "SceneManager.h"

namespace LindaEngine
{
	class Application : public IRuntimeModule
	{
	public:
		int Initialize();
		void Finalize();
		void Tick();

		static void Quit();

		static Window& GetWindow() { return *_window; }

	private:
		static bool _isQuit;

		static Scope<Window> _window;
		Scope<GraphicsContext> _graphicContext;
		Scope<SceneManager> _sceneManager;
	};
}
