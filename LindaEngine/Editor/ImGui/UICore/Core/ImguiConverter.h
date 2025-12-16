#pragma once

#include "imgui/imgui.h"
#include "glm/glm.hpp"

namespace LindaEditor
{
	class ImguiConverter
	{
	public:
		static ImVec2 ToImVec2(glm::vec2 value);
		static ImVec4 ToImVec4(glm::vec4 value);

		static glm::vec2 ToGLMVec2(ImVec2 value);
		static glm::vec4 ToGLMVec4(ImVec4 value);
	};
}