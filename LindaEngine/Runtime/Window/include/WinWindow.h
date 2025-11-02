#pragma once

#include <Windows.h>

#include "Window.h"

namespace LindaEngine
{
	class WinWindow : public Window
	{
	public:
		WinWindow();
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

		HDC GetWinDC();

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT OnEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND _hWnd;
		HDC  _hDc;
	};
}
