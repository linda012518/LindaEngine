#pragma once

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <string>

namespace LindaEditor
{
	using WidgetCallback = std::function<void()>;
	using DropdownCallback = std::function<void(int)>;

	class GUILayoutEditor
	{
	public:
		static void Checkbox(std::string name, bool* value, WidgetCallback onChanged, float nameSize = -1.0f);
		static void TextDescription(std::string name, const char* value, bool disabled, float nameSize = -1.0f);
		static void DragFloat(std::string name, float* value, WidgetCallback onChanged, float speed = 1.0f, float min = 0.0f, float max = 0.0f, float nameSize = -1.0f);
		static void DragInt(std::string name, int* value, WidgetCallback onChanged, float speed = 1, int min = 0, int max = 0, float nameSize = -1.0f);
		static void ColorEdit4(std::string name, float* value, WidgetCallback onChanged, float nameSize = -1.0f);
		static void Dropdown(std::string name, std::vector<std::string>& value, DropdownCallback onChanged);
		static void ComboSelectable(std::string name, int curIndex, std::vector<std::string>& value, DropdownCallback onChanged);

		static float ImGuiLabelColumnWidth(std::initializer_list<const char*> labels);

	private:
	};
}
