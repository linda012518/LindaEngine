#include "GUILayoutEditor.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;

float ComputeImGuiLabelColumnWidth(std::initializer_list<const char*> labels)
{
	float w = 0.f;
	for (const char* s : labels)
		w = std::max(w, ImGui::CalcTextSize(s).x);
	const float pad = ImGui::GetStyle().CellPadding.x * 2.0f;
	return w + pad;
}

template<typename UIFunction>
void DrawWidget(UIFunction uiFunction, std::string name, float nameSize)
{
	if (-1.0f == nameSize)
		nameSize = ComputeImGuiLabelColumnWidth({ name.c_str() });

	if (ImGui::BeginTable((name + "Table").c_str(), 2, ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, nameSize);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(name.c_str());
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		uiFunction();
		ImGui::EndTable();
	}
}

void GUILayoutEditor::Checkbox(std::string name, bool* value, WidgetCallback onChanged, float nameSize)
{
	DrawWidget([&]() { 
		if (ImGui::Checkbox(("##" + name).c_str(), value) && onChanged)
			onChanged();
		}, name, nameSize);
}

void GUILayoutEditor::TextDescription(std::string name, const char* value, bool disabled, float nameSize)
{
	DrawWidget([&]() {
		const float restW = std::max(4.0f, ImGui::GetContentRegionAvail().x);
		const ImGuiStyle& style = ImGui::GetStyle();
		const char* text = value ? value : "";
		const float rowH = ImGui::GetFrameHeight();
		const ImGuiChildFlags frameChild = ImGuiChildFlags_FrameStyle;

		ImGui::BeginChild((name + "TextDescFrame").c_str(), ImVec2(restW, rowH), frameChild, ImGuiWindowFlags_NoScrollbar);
		ImGui::PushStyleColor(ImGuiCol_Text, disabled ? style.Colors[ImGuiCol_TextDisabled] : style.Colors[ImGuiCol_Text]);
		ImGui::PushTextWrapPos(FLT_MAX);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
		ImGui::PopStyleColor();
		ImGui::EndChild();
		}, name, nameSize);
}

void GUILayoutEditor::DragFloat(std::string name, float* value, WidgetCallback onChanged, float speed, float min, float max, float nameSize)
{
	DrawWidget([&]() { 
		if (ImGui::DragFloat(("##" + name).c_str(), value, speed, min, max) && onChanged)
			onChanged();
		}, name, nameSize);
}

void GUILayoutEditor::DragInt(std::string name, int* value, WidgetCallback onChanged, float speed, int min, int max, float nameSize)
{
	DrawWidget([&]() {
		if (ImGui::DragInt(("##" + name).c_str(), value, speed, min, max) && onChanged)
			onChanged();
	}, name, nameSize);
}

void GUILayoutEditor::ColorEdit4(std::string name, float* value, WidgetCallback onChanged, float nameSize)
{
	DrawWidget([&]() {
		if (ImGui::ColorEdit4(("##" + name).c_str(), value) && onChanged)
			onChanged();
		}, name, nameSize);
}

void GUILayoutEditor::Dropdown(std::string name, int curIndex, std::vector<std::string>& value, DropdownCallback onChanged, float nameSize)
{
	DrawWidget([&]() {
		if (ImGui::Button(value[curIndex].c_str(), ImVec2(-1, 0))) {
			ImGui::OpenPopup("##Dropdown");
		}

		ImVec2 buttonMin = ImGui::GetItemRectMin();
		ImVec2 buttonMax = ImGui::GetItemRectMax();

		ImGui::SetNextWindowPos(ImVec2(buttonMin.x, buttonMax.y));
		ImGui::SetNextWindowSize(ImVec2(buttonMax.x - buttonMin.x, 0));

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.8f, 0.8f, 0.85f, 0.95f));    // 背景色
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));        // 边框色
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));          // 字体色

		if (ImGui::BeginPopup("##Dropdown", ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove))
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.3f, 0.6f, 0.8f));        // 选中项背景
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.4f, 0.7f, 0.8f)); // 悬停背景
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.5f, 0.8f, 1.0f));  // 点击背景

			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

			for (int i = 0; i < (int)value.size(); i++)
			{
				const bool is_selected = (curIndex == i);
				if (ImGui::Selectable(value[i].c_str(), is_selected))
				{
					if (onChanged)
						onChanged(i);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::PopStyleVar(1);

			ImGui::PopStyleColor(3);
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(3);

		}, name, nameSize);
}

void GUILayoutEditor::ComboSelectable(std::string name, int curIndex, std::vector<std::string>& value, DropdownCallback onChanged, float nameSize)
{
	DrawWidget([&]() {
		const char* preview = "";
		if (!value.empty() && curIndex >= 0 && curIndex < (int)value.size())
			preview = value[curIndex].c_str();

		const ImVec4 bgPopup(0.8f, 0.8f, 0.85f, 0.95f);
		const ImVec4 borderCol(0.3f, 0.4f, 0.8f, 1.0f);
		const ImVec4 textCol(0.2f, 0.2f, 0.2f, 1.0f);

		ImGui::SetNextItemWidth(-FLT_MIN);

		const ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular;

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.8f, 0.8f, 0.85f, 0.95f));
		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginCombo(("##" + name).c_str(), preview, flags))
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.3f, 0.6f, 0.8f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.4f, 0.7f, 0.8f));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.5f, 0.8f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Text, textCol);
			ImGui::PushStyleColor(ImGuiCol_Border, borderCol);

			for (int n = 0; n < (int)value.size(); n++)
			{
				const bool is_selected = (curIndex == n);
				if (ImGui::Selectable(value[n].c_str(), is_selected))
				{
					if (onChanged)
						onChanged(n);
				}
			}

			ImGui::PopStyleColor(5);
			ImGui::EndCombo();
		}

		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);

		}, name, nameSize);
}

void GUILayoutEditor::Vec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.26f, 0.26f, 0.26f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.36f, 0.36f, 0.36f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.56f, 0.56f, 0.56f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void GUILayoutEditor::PopupContextMenu(WidgetCallback enable, WidgetCallback disable)
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));       // 菜单栏背景
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));            // 文本颜色
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));         // 弹出菜单背景
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));          // 选中状态（子菜单打开时）

	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.75f, 0.75f, 0.75f, 1.0f));   // 悬停背景
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));    // 激活背景
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));          // 边框颜色
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));       // 分隔线颜色

	if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
	{
		if (enable) enable();
		ImGui::EndPopup();
	}
	else
	{
		if (disable) disable();
	}

	ImGui::PopStyleColor(8);

}

void GUILayoutEditor::Dropdown(std::string name, std::vector<std::string>& value, DropdownCallback onChanged)
{
	if (ImGui::Button(name.c_str(), ImVec2(-1, 0))) {
		ImGui::OpenPopup("##Dropdown");
	}

	ImVec2 buttonMin = ImGui::GetItemRectMin();
	ImVec2 buttonMax = ImGui::GetItemRectMax();

	ImGui::SetNextWindowPos(ImVec2(buttonMin.x, buttonMax.y));
	ImGui::SetNextWindowSize(ImVec2(buttonMax.x - buttonMin.x, 0));

	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.8f, 0.8f, 0.85f, 0.95f));    // 背景色
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));        // 边框色
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));          // 字体色

	if (ImGui::BeginPopup("##Dropdown", ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove))
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.3f, 0.6f, 0.8f));        // 选中项背景
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.4f, 0.7f, 0.8f)); // 悬停背景
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.5f, 0.8f, 1.0f));  // 点击背景

		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

		for (int i = 0; i < (int)value.size(); i++)
		{
			if (ImGui::Selectable(value[i].c_str()))
			{
				if (onChanged)
					onChanged(i);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::PopStyleVar(1);

		ImGui::PopStyleColor(3);
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor(3);
}

float GUILayoutEditor::ImGuiLabelColumnWidth(std::initializer_list<const char*> labels)
{
	return ComputeImGuiLabelColumnWidth(labels);
}

