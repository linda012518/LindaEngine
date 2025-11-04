#pragma once

#include <glm/glm.hpp>

#include "InputCodes.h"

namespace LindaEngine
{
	class Input
	{
	public:
		static bool GetKeyDown(KeyCode key);
		static bool GetKey(KeyCode key);
		static bool GetKeyUp(KeyCode key);
		static bool GetMouseButtonDown(MouseCode button);
		static bool GetMouseButton(MouseCode button);
		static bool GetMouseButtonUp(MouseCode button);
		static glm::ivec2 GetMousePosition();
	};
}
