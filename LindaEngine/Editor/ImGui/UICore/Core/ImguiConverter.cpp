#include "ImguiConverter.h"

ImVec2 LindaEditor::ImguiConverter::ToImVec2(glm::vec2 value)
{
	return ImVec2(value.x, value.y);
}

ImVec4 LindaEditor::ImguiConverter::ToImVec4(glm::vec4 value)
{
	return ImVec4(value.x, value.y, value.z, value.w);
}

glm::vec2 LindaEditor::ImguiConverter::ToGLMVec2(ImVec2 value)
{
	return glm::vec2(value.x, value.y);
}

glm::vec4 LindaEditor::ImguiConverter::ToGLMVec4(ImVec4 value)
{
	return glm::vec4(value.x, value.y, value.z, value.w);
}
