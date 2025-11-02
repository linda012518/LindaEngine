#include "Input.h"

using namespace LindaEngine;

bool Input::GetKeyDown(KeyCode key)
{
	return false;
}

bool Input::GetKeyPressed(KeyCode key)
{
	return false;
}

bool Input::GetKeyUp(KeyCode key)
{
	return false;
}

bool Input::GetMouseButtonDown(MouseCode button)
{
	return false;
}

bool Input::GetMouseButtonPressed(MouseCode button)
{
	return false;
}

bool Input::GetMouseButtonUp(MouseCode button)
{
	return false;
}

glm::vec2 Input::GetMousePosition()
{
	return glm::vec2();
}

float Input::GetMouseX()
{
	return 0.0f;
}

float Input::GetMouseY()
{
	return 0.0f;
}
