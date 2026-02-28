#pragma once

#include "IRuntimeModule.h"

#include "Window.h"
#include "GraphicsContext.h"
#include "AppState.h"

namespace LindaEngine
{
	class Application : public IRuntimeModule
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

		void SetFrameRate(int rate);

		static void Quit();
		static Window& GetWindow() { return *_window; }
		static AppState state;
		static AppModule module;

	protected:
		virtual void OnPreRender() { }

	protected:
		static bool _isQuit;
		static Scope<Window> _window;
		Scope<GraphicsContext> _graphicContext;

		int _frameRate = -1;
		float _frameInterval = -1.0f;
	};
}
