#include "Texture.h"
#include "TextureManager.h"
#include "TextureDriver.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Path.h"
#include "GUILayoutEditor.h"
#include "EventEditor.h"
#include "EventCodeEditor.h"
#include "EventSystemEditor.h"

#include "imgui/imgui.h"
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

using namespace LindaEngine;
using namespace LindaEditor;

TextureType GetTextureTypeByString(std::string type)
{
	if (type == "Cube") return TextureType::Cube;
	else if (type == "RenderTexture") return TextureType::RenderTexture;
	else if (type == "Tex2D") return TextureType::Tex2D;
	return TextureType::Tex2D;
}

FilterMode GetFilterModeByString(std::string type)
{
	if (type == "Point") return FilterMode::Point;
	else if (type == "Bilinear") return FilterMode::Bilinear;
	else if (type == "Trilinear") return FilterMode::Trilinear;
	return FilterMode::Bilinear;
}

TextureWrapMode GetWarpModeByString(std::string type)
{
	if (type == "Clamp") return TextureWrapMode::Clamp;
	else if (type == "Repeat") return TextureWrapMode::Repeat;
	else if (type == "Mirror") return TextureWrapMode::Mirror;
	return TextureWrapMode::Clamp;
}

Ref<Texture> Texture::overrideTexture = nullptr;

Ref<RenderTexture> RenderTexture::active = nullptr;
Ref<RenderTexture> RenderTexture::finalRT = nullptr;

void Texture::OnImguiRender(Texture* texture)
{
	std::string id = texture->path;
	ImGui::PushID(id.c_str());

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	if (ImGui::BeginTable("TextureImGuiHeader", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("icon", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("fields", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		if (texture->type == TextureType::Tex2D)
			ImGui::Image((ImTextureID)(uintptr_t)texture->nativeColorID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
		else if (texture->type == TextureType::Cube)
		{
			static Ref<Material> material = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/CubemapVisible.shader");
			std::shared_ptr<Texture> tmp(texture, [](Texture*) { });
			material->SetUniformValue("skybox", tmp);
			Ref<RenderTexture> rt = TextureDriver::RenderMaterialBall(material);
			ImGui::Image((ImTextureID)(uintptr_t)rt->nativeIDs[0], ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::TableSetColumnIndex(1);
		ImGui::TextUnformatted(("Name : " + Path::GetFileNameNoExtension(texture->path)).c_str());
		ImGui::TextUnformatted(("Width : " + std::to_string(texture->width)).c_str());
		ImGui::TextUnformatted(("Height : " + std::to_string(texture->height)).c_str());

		ImGui::EndTable();
	}

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "Texture Shape", "Mipmap Count", "Anisotropy", "Filter Mode", "WarpW Mode"});
	float firstButtonHeight = ImGui::GetFrameHeight();

	static std::vector<std::string> namesTexture = { "Tex2D", "Cube" };
	GUILayoutEditor::Dropdown("Texture Shape", (int)texture->type_temp, namesTexture, [&](int index) {
		texture->type_temp = GetTextureTypeByString(namesTexture[index]);
		}, firstWidth);

	GUILayoutEditor::Checkbox("sRGB", &texture->isGammaCorrection_temp, nullptr, firstWidth);

	GUILayoutEditor::DragInt("Mipmap Count", &texture->mipmapCount_temp, nullptr, 1.0f, 1, 6, firstWidth);

	GUILayoutEditor::DragInt("Anisotropy", &texture->anisotropy_temp, nullptr, 1.0f, 1, 8, firstWidth);

	static std::vector<std::string> namesFilter = { "Point", "Bilinear", "Trilinear" };
	GUILayoutEditor::Dropdown("Filter Mode", (int)texture->filter_temp, namesFilter, [&](int index) {
		texture->filter_temp = GetFilterModeByString(namesFilter[index]);
		}, firstWidth);

	static std::vector<std::string> names = { "Repeat", "Clamp", "Mirror" };
	GUILayoutEditor::Dropdown("WarpU Mode", (int)texture->warpU_temp, names, [&](int index) {
		texture->warpU_temp = GetWarpModeByString(names[index]);
		}, firstWidth);

	GUILayoutEditor::Dropdown("WarpV Mode", (int)texture->warpV_temp, names, [&](int index) {
		texture->warpV_temp = GetWarpModeByString(names[index]);
		}, firstWidth);

	GUILayoutEditor::Dropdown("WarpW Mode", (int)texture->warpW_temp, names, [&](int index) {
		texture->warpW_temp = GetWarpModeByString(names[index]);
		}, firstWidth);

	float availableWidth = ImGui::GetContentRegionAvail().x;
	float spacing = ImGui::GetStyle().ItemSpacing.x;
	float componentWidth = (availableWidth - spacing) / 2.0f;

	if (ImGui::Button("Apply", ImVec2(componentWidth, firstButtonHeight)))
	{
		texture->type = texture->type_temp;
		texture->isGammaCorrection = texture->isGammaCorrection_temp;
		texture->mipmapCount = texture->mipmapCount_temp;
		texture->anisotropy = texture->anisotropy_temp;
		texture->filter = texture->filter_temp;
		texture->warpU = texture->warpU_temp;
		texture->warpV = texture->warpV_temp;
		texture->warpW = texture->warpW_temp;

		Ref<Texture> newTexture = TextureManager::GetTexture(texture->path, true);

		// TODO 暂时先这样处理
		SwitchInspectorObjectEditor sio;
		sio.lobject = newTexture.get();
		EventSystemEditor::Dispatch(nullptr, EventCodeEditor::SwitchInspectorObject, sio);
	}

	ImGui::SameLine();

	if (ImGui::Button("Revert", ImVec2(componentWidth, firstButtonHeight)))
	{
		texture->type_temp = texture->type;
		texture->isGammaCorrection_temp = texture->isGammaCorrection;
		texture->mipmapCount_temp = texture->mipmapCount;
		texture->anisotropy_temp = texture->anisotropy;
		texture->filter_temp = texture->filter;
		texture->warpU_temp = texture->warpU;
		texture->warpV_temp = texture->warpV;
		texture->warpW_temp = texture->warpW;
	}

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	ImGui::PopID();
}