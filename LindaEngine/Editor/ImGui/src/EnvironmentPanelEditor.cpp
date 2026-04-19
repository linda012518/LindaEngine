#include "EnvironmentPanelEditor.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Texture.h"
#include "TextureManager.h"
#include "GUILayoutEditor.h"
#include "Path.h"
#include "Environment.h"
#include "ContentBrowserPanelEditor.h"
#include "RenderPipelineEditor.h"
#include "PostProcessEffectRenderer.h"
#include "Camera.h"
#include "Application.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

using namespace LindaEditor;
using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(EnvironmentPanelEditor, ImGuiPanelEditor)

void EnvironmentPanelEditor::OnImGuiRender()
{
	ImGui::Begin("  Environment  ");

	DrawSkyboxMaterial();
	DrawPostProcessEffect();
	DrawLayerMaskList();

	ImGui::End();

}

void EnvironmentPanelEditor::DrawSkyboxMaterial()
{
	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "Skybox Material" }) * 1.2f;

	Ref<Texture> matIconTex = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Material.png");
	const ImTextureID matIconId = matIconTex ? (ImTextureID)(uintptr_t)matIconTex->nativeColorID : (ImTextureID)0;

	if (ImGui::BeginTable("MaterialsTable", 2, ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.26f, 0.26f, 0.26f, 0.0f }); // 透明背景
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.36f, 0.36f, 0.36f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.56f, 0.56f, 0.56f, 0.0f });
		ImGui::Button("Skybox Material");
		ImGui::PopStyleColor(3);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);

		const bool hasMaterial = Environment::GetSkyboxMaterial() != nullptr;
		std::string display = hasMaterial
			? Path::GetFileNameNoExtension(Environment::GetSkyboxMaterial()->GetPath())
			: "(Drag material)";

		const ImGuiStyle& style = ImGui::GetStyle();
		const float rowHeight = ImGui::GetFrameHeight();
		const ImVec2 availSize = ImVec2(ImGui::GetContentRegionAvail().x, rowHeight);

		ImVec4 bgColor = style.Colors[ImGuiCol_FrameBg];
		ImVec4 bgHover = style.Colors[ImGuiCol_FrameBgHovered];
		ImVec4 bgActive = style.Colors[ImGuiCol_FrameBgActive];

		ImGui::PushStyleColor(ImGuiCol_Header, bgColor);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, bgColor);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, bgActive);

		bool selected = false;
		if (ImGui::Selectable("##matSlot", false, ImGuiSelectableFlags_Highlight, availSize))
		{
			// 点击事件
		}

		ImGui::PopStyleColor(3);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILENODE_DRAG"))
			{
				LindaEditor::FileNode* node = *(LindaEditor::FileNode**)payload->Data;
				if (node && node->type == LindaEditor::FileType::Material)
				{
					Environment::SetSkybox(MaterialManager::GetMaterial(node->path, false));
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (hasMaterial && ImGui::BeginPopupContextItem("##ClearMaterial"))
		{
			if (ImGui::MenuItem("Clear Material"))
			{
				//Environment::SetSkybox(nullptr);
				//ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImVec2 itemMin = ImGui::GetItemRectMin();
		ImVec2 itemMax = ImGui::GetItemRectMax();
		float itemHeight = itemMax.y - itemMin.y;
		float contentY = itemMin.y + (itemHeight - 18.0f) * 0.5f; // 18是图标高度

		float x = itemMin.x + style.FramePadding.x;
		ImGui::SetCursorScreenPos(ImVec2(x, contentY));

		if (matIconId && hasMaterial)
		{
			ImGui::Image(matIconId, ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine(0, 6);
		}

		ImVec2 textSize = ImGui::CalcTextSize(display.c_str());
		float textY = itemMin.y + (itemHeight - textSize.y) * 0.5f;
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, textY));

		if (!hasMaterial)
			ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]);

		ImGui::TextUnformatted(display.c_str());

		if (!hasMaterial)
			ImGui::PopStyleColor();

		ImGui::EndTable();
	}
}

void EnvironmentPanelEditor::DrawPostProcessEffect()
{
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	std::vector<Ref<PostProcessEffectRenderer>>& postStack = RenderPipelineEditor::activeCamera->GetPostStack();

	static std::vector<std::string> postprocessNames = PostProcessEffectRenderer::GetPostProcessNames();
	GUILayoutEditor::Dropdown("Add PostProcess", postprocessNames, [&](int index) {
		bool has = false;
		for (auto& go : postStack)
		{
			if (go->GetClassName() != postprocessNames[index])
				continue;
			has = true;
			break;
		}
		if (false == has)
			RenderPipelineEditor::activeCamera->AddPostProcess(postprocessNames[index]);
		});

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	if (postStack.size() <= 0)
		return;

	for (auto& go : postStack)
	{
		if (nullptr == go)
			continue;

		ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, 0);

		ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 1.0f, 1.0f, 1.00f));

		std::string goID = "##" + std::string((const char*)go.get());
		bool ret = go->IsEnable();
		ImGui::Checkbox(goID.c_str(), &ret);
		if (go->IsEnable() != ret)
			go->SetEnable(ret);

		ImGui::PopStyleVar(1);
		ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, 1);

		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::SameLine();

		ImGui::PopStyleColor(1);

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.26f, 0.26f, 0.26f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.36f, 0.36f, 0.36f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.56f, 0.56f, 0.56f, 1.00f));

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (ImGui::TreeNodeEx((void*)go.get(), treeNodeFlags, go->GetClassName().c_str()))
		{
			if (ImGui::IsItemHovered())
				_postProcessDirty = go;
			ImGui::TreePop();
		}

		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(3);
	}

	GUILayoutEditor::PopupContextMenu(
		[&]() {
			if (ImGui::MenuItem("Remove PostProcess"))
			{
				postStack.erase(std::remove(postStack.begin(), postStack.end(), _postProcessDirty), postStack.end());
				_postProcessDirty = nullptr;
			}
		}, nullptr);


	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

}

void EnvironmentPanelEditor::DrawLayerMaskList()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)this, flags, "All Layers");
	if (false == opened)
	{
		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
		ImGui::PopStyleColor(1);
		return;
	}

	auto& map = Application::layerToNameMap;
	for (auto& pair : map)
	{
		if (pair.first <= 0)
			continue;
		ImGui::SetNextItemWidth(-FLT_MIN);
		std::string inputLabel = "##Name" + std::to_string(pair.first);
		ImGui::InputText(inputLabel.c_str(), &pair.second);
	}
	ImGui::TreePop();

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);
}
