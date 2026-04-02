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

void GUILayoutEditor::Dropdown(std::string name, std::vector<std::string>& value, DropdownCallback onChanged)
{
	static bool show_popup = false;
	if (ImGui::Button(name.c_str(), ImVec2(-1, 0))) {
		show_popup = true;
	}
	if (show_popup)
	{
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.85f, 0.95f));    // 背景色：深蓝灰
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));        // 边框色：蓝色
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));          // 字体色：浅灰

		ImGui::Begin("##Dropdown", &show_popup,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize);

		// 检测点击外部区域
		bool popup_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup |
			ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		bool button_hovered = ImGui::IsItemHovered();

		// 如果没有悬停在弹出框或按钮上，且鼠标被点击，则关闭弹出框
		if (ImGui::IsMouseClicked(0) && !popup_hovered && !button_hovered) {
			show_popup = false;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			show_popup = false;
		}

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.3f, 0.6f, 0.8f));        // 选中项背景
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.4f, 0.7f, 0.8f)); // 悬停背景
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.5f, 0.8f, 1.0f));  // 点击背景

		for (int i = 0; i < (int)value.size(); i++)
		{
			if (ImGui::Selectable(value[i].c_str()))
			{
				show_popup = false;
				onChanged(i);
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::End();
		ImGui::PopStyleColor(3);

	}

}

void GUILayoutEditor::ComboSelectable(std::string name, int curIndex, std::vector<std::string>& value, DropdownCallback onChanged)
{
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
}

float GUILayoutEditor::ImGuiLabelColumnWidth(std::initializer_list<const char*> labels)
{
	return ComputeImGuiLabelColumnWidth(labels);
}

