#pragma once

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <string>
#include <vector>
#include <map>

#include "glm/glm.hpp"

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
		static void DragInt4(std::string name, int* value, WidgetCallback onChanged, float speed = 1, int min = 0, int max = 0, float nameSize = -1.0f);
		static void ColorEdit4(std::string name, float* value, WidgetCallback onChanged, float nameSize = -1.0f, bool isHDR = false);
		static void Dropdown(std::string name, int curIndex, std::vector<std::string>& value, DropdownCallback onChanged, float nameSize = -1.0f);
		static void DropdownCheckbox(std::string name, int curIndex, std::map<int, std::string>& value, DropdownCallback onChanged, float nameSize = -1.0f);
		static void ComboSelectable(std::string name, int curIndex, std::vector<std::string>& value, DropdownCallback onChanged, float nameSize = -1.0f);
		static void ComboInputText(std::string name, int curIndex, std::vector<std::string>& value, DropdownCallback onChanged, float nameSize = -1.0f);
		static void Vec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DragFloat4(std::string name, float* value, WidgetCallback onChanged, float speed = 1.0f, float min = 0.0f, float max = 0.0f, float nameSize = -1.0f);

		static void ConfirmWindow(std::string title, std::string message, std::string cancelText, std::string confirmText, WidgetCallback callback);
		static void PopupContextMenu(WidgetCallback enable, WidgetCallback disable);
		static void Dropdown(std::string name, std::vector<std::string>& value, DropdownCallback onChanged);
		
		static void DrawConfirmWindow();
		static float ImGuiLabelColumnWidth(std::vector<const char*> labels);

	private:
		static bool _isOpen;
		static std::string _title;
		static std::string _message;
		static std::string _cancelText;
		static std::string _confirmText;
		static WidgetCallback _callback;
	};
}
