#pragma once

#include <Windows.h>
#include <chrono>

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
		void OnTimer(WPARAM wParam, LPARAM lParam);
		void MouseButtonDown(LPARAM lParam, bool isLeft);
		void MouseButtonUp(LPARAM lParam, bool isLeft);
	private:
		HWND _hWnd;
		HDC  _hDc;
		bool _leftButtonHeld = false;
		bool _rightButtonHeld = false;
		bool _keyHeld = false;
		const int CLICK_TIME_THRESHOLD = 500; // ∫¡√Î
		const int CLICK_DISTANCE_THRESHOLD = 5; // œÒÀÿ
		POINT _downPoint;
		std::chrono::steady_clock::time_point _buttonStartTime;
		std::chrono::steady_clock::time_point _keyStartTime;
	};
}
