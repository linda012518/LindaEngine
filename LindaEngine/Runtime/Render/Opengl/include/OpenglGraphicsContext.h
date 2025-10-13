#pragma once

#include "GraphicsContext.h"

#include <Windows.h>

namespace LindaEngine
{
	class WinWindow;
	class OpenglGraphicsContext : public GraphicsContext
	{
	public:
		OpenglGraphicsContext(WinWindow* window);

		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

	protected:
		virtual void CreateRenderPipeline();

	private:
		HGLRC _renderContext;
		HDC  _hDc;
		WinWindow* _window;
	};
}
