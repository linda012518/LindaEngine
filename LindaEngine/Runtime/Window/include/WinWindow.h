#pragma once

#include <Windows.h>
#include <chrono>
#include <glm/glm.hpp>
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

		int GetKeyDown() { return _keyDwon; }
		int GetKey() { return _key; }
		int GetKeyUp() { return _keyUp; }
		bool GetMouseButtonDown(bool isLeft) { return isLeft ? _leftButtonDown : _rightButtonDown; }
		bool GetMouseButtonUp(bool isLeft) { return isLeft ? _leftButtonUp : _rightButtonUp; }
		bool GetMouseButton(bool isLeft) { return isLeft ? _leftButton : _rightButton; }
		glm::ivec2 GetMousePosition() { return _mousePos; }

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT OnEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void OnTimer(WPARAM wParam, LPARAM lParam);
		void CheckMouseButton(LPARAM lParam);
		void CheckMouseWheelPressed(LPARAM lParam);
		void MouseButtonDown(LPARAM lParam, bool isLeft);
		void MouseButtonUp(LPARAM lParam, bool isLeft);

	private:
		HWND _hWnd;
		HDC  _hDc;

		bool _mouseTracking = false;

		int _keyDwon = -1;
		int _keyUp = -1;
		int _key = -1;
		bool _leftButtonDown = false;
		bool _rightButtonDown = false;
		bool _leftButtonUp = false;
		bool _rightButtonUp = false;
		bool _leftButton = false;
		bool _rightButton = false;
		glm::ivec2 _mousePos;

		bool _leftButtonHeld = false;
		bool _rightButtonHeld = false;
		bool _wheelHeld = false;
		bool _keyHeld = false;
		const int CLICK_TIME_THRESHOLD = 500; // ∫¡√Î
		const int CLICK_DISTANCE_THRESHOLD = 5; // œÒÀÿ
		POINT _downPoint;
		std::chrono::steady_clock::time_point _buttonStartTime;
		std::chrono::steady_clock::time_point _keyStartTime;
	};
}
