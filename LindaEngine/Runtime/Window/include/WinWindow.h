#pragma once

#include <Windows.h>

#include "Window.h"
#include "GfxConfiguration.h"

namespace LindaEngine
{
	class WinWindow : public Window
	{
	public:
		WinWindow(GfxConfiguration& gfx);
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

		HDC GetWinDC();
		GfxConfiguration& GetGfxConfig();

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND _hWnd;
		HDC  _hDc;
		GfxConfiguration _gfx;
	};
}
