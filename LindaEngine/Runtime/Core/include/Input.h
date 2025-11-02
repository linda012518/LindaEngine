#pragma once

#include <glm/glm.hpp>

#include "InputCodes.h"

namespace LindaEngine
{
	class Input
	{
	public:
		static bool GetKeyDown(KeyCode key);
		static bool GetKeyPressed(KeyCode key);
		static bool GetKeyUp(KeyCode key);
		static bool GetMouseButtonDown(MouseCode button);
		static bool GetMouseButtonPressed(MouseCode button);
		static bool GetMouseButtonUp(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
