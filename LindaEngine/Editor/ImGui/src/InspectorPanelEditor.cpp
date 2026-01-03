#include "InspectorPanelEditor.h"
#include "EventSystemEditor.h"
#include "Entity.h"
#include "EventCodeEditor.h"
#include "glm/glm.hpp"
#include "Transform.h"
#include "Component.h"

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
	bool ret = true;
	ImGui::Checkbox(go.c_str(), &ret);

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

	DrawComponents();

	ImGui::End();
}

void InspectorPanelEditor::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	SwitchSelectEntityEditor& event = dynamic_cast<SwitchSelectEntityEditor&>(eventData);
	_selectionEntity = event.selectionEntity;
}

void InspectorPanelEditor::DrawComponents()
{
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
				if (ImGui::IsItemHovered())
				{
					_component = go.get();
					printf(_component->GetClassName().c_str());
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


