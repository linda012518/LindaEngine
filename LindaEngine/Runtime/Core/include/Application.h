#pragma once

#include "IRuntimeModule.h"

#include "Window.h"
#include "GraphicsContext.h"
#include "SceneManager.h"

namespace LindaEngine
{
	enum class AppState
	{
		Loading, Running, PrepareQuit
	};

	class Application : public IRuntimeModule
	{
	public:
		int Initialize();
		void Finalize();
		void Tick();

		static void Quit();

		static Window& GetWindow() { return *_window; }

		static AppState state;

	private:
		static bool _isQuit;

		static Scope<Window> _window;
		Scope<GraphicsContext> _graphicContext;
		Scope<SceneManager> _sceneManager;
	};
}
