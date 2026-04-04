#include "Texture.h"
#include "GUILayoutEditor.h"
#include "TextureManager.h"
#include "Path.h"

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

void Texture::OnImguiRender()
{
	ImGui::PushID(this);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	if (ImGui::BeginTable("TextureImGuiHeader", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("icon", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("fields", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Image((ImTextureID)(uintptr_t)nativeColorID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TableSetColumnIndex(1);
		ImGui::TextUnformatted(("Name : " + Path::GetFileNameNoExtension(path)).c_str());
		ImGui::TextUnformatted(("Width : " + std::to_string(width)).c_str());
		ImGui::TextUnformatted(("Height : " + std::to_string(height)).c_str());

		ImGui::EndTable();
	}

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "Texture Shape", "Mipmap Count", "Anisotropy", "Filter Mode", "WarpW Mode"});
	float firstButtonHeight = ImGui::GetFrameHeight();

	static std::vector<std::string> namesTexture = { "Tex2D", "Cube" };
	GUILayoutEditor::Dropdown("Texture Shape", (int)type, namesTexture, [&](int index) {
		type = GetTextureTypeByString(namesTexture[index]);
		}, firstWidth);

	GUILayoutEditor::Checkbox("sRGB", &isGammaCorrection, nullptr, firstWidth);

	GUILayoutEditor::DragInt("Mipmap Count", &mipmapCount, nullptr, 1.0f, 1, 6, firstWidth);

	GUILayoutEditor::DragInt("Anisotropy", &anisotropy, nullptr, 1.0f, 1, 8, firstWidth);

	static std::vector<std::string> namesFilter = { "Point", "Bilinear", "Trilinear" };
	GUILayoutEditor::Dropdown("Filter Mode", (int)filter, namesFilter, [&](int index) {
		filter = GetFilterModeByString(namesFilter[index]);
		}, firstWidth);

	static std::vector<std::string> names = { "Repeat", "Clamp", "Mirror" };
	GUILayoutEditor::Dropdown("WarpU Mode", (int)warpU, names, [&](int index) {
		warpU = GetWarpModeByString(names[index]);
		}, firstWidth);

	GUILayoutEditor::Dropdown("WarpV Mode", (int)warpV, names, [&](int index) {
		warpV = GetWarpModeByString(names[index]);
		}, firstWidth);

	GUILayoutEditor::Dropdown("WarpW Mode", (int)warpW, names, [&](int index) {
		warpW = GetWarpModeByString(names[index]);
		}, firstWidth);

	float availableWidth = ImGui::GetContentRegionAvail().x;
	float spacing = ImGui::GetStyle().ItemSpacing.x;
	float componentWidth = (availableWidth - spacing) / 2.0f;

	if (ImGui::Button("Apply", ImVec2(componentWidth, firstButtonHeight)))
	{
		// TODO 更新纹理
	}

	ImGui::SameLine();

	if (ImGui::Button("Revert", ImVec2(componentWidth, firstButtonHeight)))
	{
		// TODO 恢复纹理
	}

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	ImGui::PopID();
}