#include "InspectorPanelEditor.h"
#include "EventSystemEditor.h"
#include "Entity.h"
#include "EventCodeEditor.h"
#include "glm/glm.hpp"
#include "Transform.h"
#include "Component.h"
#include "ComponentFactory.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace LindaEditor;
using namespace LindaEngine;

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
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


template<typename T, typename UIFunction>
static void DrawComponent(T* component, Entity* entity, UIFunction uiFunction)
{
	bool isTransform = std::is_same<Transform, T>::value;

	ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, 0);
	
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 1.0f, 1.0f, 1.00f));
	if (isTransform)
		ImGui::BeginDisabled();

	std::string go = "##" + std::string((const char *)component);
	bool ret = component->GetEnable();
	ImGui::Checkbox(go.c_str(), &ret);
	if (component->GetEnable() != ret)
		component->SetEnable(ret);

	ImGui::PopStyleVar(1);
	ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, 1);

	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	if (isTransform)
		ImGui::EndDisabled();
	ImGui::PopStyleColor(1);

	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.26f, 0.26f, 0.26f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.36f, 0.36f, 0.36f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.56f, 0.56f, 0.56f, 1.00f));

	ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
	treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (ImGui::TreeNodeEx((void*)component, treeNodeFlags, component->GetClassName().c_str()))
	{
		uiFunction(component);
		ImGui::TreePop();
	}

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(3);
}

DYNAMIC_CREATE_CLASS(InspectorPanelEditor, ImGuiPanelEditor)

InspectorPanelEditor::InspectorPanelEditor()
{
	EventSystemEditor::Bind(EventCodeEditor::SwitchSelectEntity, this);
}

void InspectorPanelEditor::OnImGuiRender()
{
	bool close = true;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Inspector", &close, window_flags);

	if (nullptr == _selectionEntity)
	{
		ImGui::End();
		return;
	}

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	bool ret = _selectionEntity->IsActiveSelf();
	ImGui::Checkbox(std::string("##" + _selectionEntity->GetName()).c_str(), &ret);
	if (_selectionEntity->IsActiveSelf() != ret)
		_selectionEntity->SetActive(ret);

	ImGui::SameLine();
	static char name[256] = "";
	auto go = _selectionEntity->GetName().c_str();
	strncpy_s(name, go, 256);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18.0f);
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::InputText("##Name", name, IM_ARRAYSIZE(name));

	DrawComponents();
	DrawSundry();

	ImGui::End();
}

void InspectorPanelEditor::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	SwitchSelectEntityEditor& event = dynamic_cast<SwitchSelectEntityEditor&>(eventData);
	_selectionEntity = event.selectionEntity;
}

void InspectorPanelEditor::DrawComponents()
{
	if (nullptr == _selectionEntity)
		return;

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	std::vector<Ref<Component>>& coms = _selectionEntity->GetAllComponent();
	for (auto& go : coms)
	{
		Transform* pointer = dynamic_cast<Transform*>(go.get());
		if (nullptr != pointer)
		{
			DrawComponent<Transform>(pointer, _selectionEntity, [](auto& com)
			{
				//ImGui::Separator();
				glm::vec3& pos = com->GetLocalPosition();
				DrawVec3Control("Position", pos);
				com->SetLocalPosition(pos);

				ImGui::Spacing();
				glm::vec3& angles = com->GetLocalEulerAngles();
				DrawVec3Control("Angles", angles);
				com->SetLocalEulerAngles(angles);

				ImGui::Spacing();
				glm::vec3& scale = com->GetLocalScale();
				DrawVec3Control("Scale", scale, 1.0f);
				com->SetLocalScale(scale);
			});
		}
		else
		{
			DrawComponent<Component>(go.get(), _selectionEntity, [=](auto& com)
			{
				go->OnImguiRender();
				if (ImGui::IsItemHovered())
				{
					_component = go.get();
				}
			});
		}
		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
		ImGui::PopStyleColor(1);
	}

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
		if (ImGui::MenuItem("Remove component"))
		{
			_selectionEntity->RemoveComponentImmediately(_component);
			_component = nullptr;
		}
		ImGui::EndPopup();
	}
	else
	{
		_component = nullptr;
	}

	ImGui::PopStyleColor(8);
}

void InspectorPanelEditor::DrawSundry()
{
	std::vector<std::string>& names = ComponentFactory::GetComponents();

	static bool show_popup = false;

	if (ImGui::Button("Add Component", ImVec2(-1, 0))) {
		show_popup = true;
	}

	if (false == show_popup)
		return;

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

	for (int i = 0; i < (int)names.size(); i++)
	{
		if (ImGui::Selectable(names[i].c_str()))
		{
			show_popup = false;
			_selectionEntity->AddComponent(names[i]);
		}
	}

	ImGui::PopStyleColor(3);
	ImGui::End();
	ImGui::PopStyleColor(3);
}


