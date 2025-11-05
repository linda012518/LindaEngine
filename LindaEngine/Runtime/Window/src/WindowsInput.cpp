#include "Input.h"
#include "Application.h"
#include "WinWindow.h"
#include "EventCode.h"
#include "EventSystem.h"

using namespace LindaEngine;

bool Input::GetKeyDown(KeyCode key)
{
	Window& win = Application::GetWindow();
	WinWindow& winWindow = dynamic_cast<WinWindow&>(win);
	return key == winWindow.GetKeyDown();
}

bool Input::GetKey(KeyCode key)
{
	Window& win = Application::GetWindow();
	WinWindow& winWindow = dynamic_cast<WinWindow&>(win);
	return key == winWindow.GetKey();
}

bool Input::GetKeyUp(KeyCode key)
{
	Window& win = Application::GetWindow();
	WinWindow& winWindow = dynamic_cast<WinWindow&>(win);
	return key == winWindow.GetKeyUp();
}

bool Input::GetMouseButtonDown(MouseCode button)
{
	Window& win = Application::GetWindow();
	WinWindow& winWindow = dynamic_cast<WinWindow&>(win);
	return winWindow.GetMouseButtonDown(button == MouseCode::ButtonLeft);
}

bool Input::GetMouseButton(MouseCode button)
{
	Window& win = Application::GetWindow();
	WinWindow& winWindow = dynamic_cast<WinWindow&>(win);
	return winWindow.GetMouseButton(button == MouseCode::ButtonLeft);
}

bool Input::GetMouseButtonUp(MouseCode button)
{
	Window& win = Application::GetWindow();
	WinWindow& winWindow = dynamic_cast<WinWindow&>(win);
	return winWindow.GetMouseButtonUp(button == MouseCode::ButtonLeft);
}

glm::ivec2 Input::GetMousePosition()
{
	Window& win = Application::GetWindow();
	WinWindow& winWindow = dynamic_cast<WinWindow&>(win);
	return winWindow.GetMousePosition();
}


