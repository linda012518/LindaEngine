#include "Material.h"
#include "ShaderManager.h"
#include "MaterialPass.h"
#include "YamlSerializer.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "Environment.h"
#include "GUILayoutEditor.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Path.h"
#include "ContentBrowserPanelEditor.h"
#include "TextureDriver.h"

#include "imgui/imgui.h"
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

#define IMPLEMENT_SETMATERIALUNIFORM(dataType) \
template<> \
void Material::SetUniformValue<dataType>(const char* name, dataType val, int count) \
{ \
	for (auto& pass : _passes) \
	{ \
		pass->SetUniformValue<dataType>(name, val, count); \
	} \
}

using namespace LindaEngine;
using namespace LindaEditor;

Ref<Material> Material::overrideMat = nullptr;
std::string Material::overrideLightMode = "Color";
bool Material::isPickPass = false;

Material::Material()
{
}

Material::~Material()
{
}

void Material::SetTexture(const std::string& name, std::string value, int pass)
{
	_passes[pass]->SetUniformValue<const char*>(name.c_str(), value.c_str());
}

void Material::SetTexture(const std::string& name, Ref<Texture> value, int pass, int renderTextureColorIndex)
{
	_passes[pass]->SetUniformValue<Ref<Texture>>(name.c_str(), value, renderTextureColorIndex);
}

void Material::SetInt(const std::string& name, int value, int pass)
{
	_passes[pass]->SetUniformValue<int>(name.c_str(), value);
}

void Material::SetFloat(const std::string& name, float value, int pass)
{
	_passes[pass]->SetUniformValue<float>(name.c_str(), value);
}

void Material::SetIntArray(const std::string& name, int count, int* value, int pass)
{
	_passes[pass]->SetUniformValue<int*>(name.c_str(), value, count);
}

void Material::SetFloatArray(const std::string& name, int count, float* value, int pass)
{
	_passes[pass]->SetUniformValue<float*>(name.c_str(), value, count);
}

void Material::SetIVec4(const std::string& name, const glm::ivec4& value, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::ivec4>(name.c_str(), value);
}

void Material::SetVec4(const std::string& name, const glm::vec4& value, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::vec4>(name.c_str(), value);
}

void Material::SetMat2(const std::string& name, const glm::mat2& mat, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::mat2>(name.c_str(), mat);
}

void Material::SetMat3(const std::string& name, const glm::mat3& mat, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::mat3>(name.c_str(), mat);
}

void Material::SetMat4(const std::string& name, const glm::mat4& mat, int count, int pass)
{
	_passes[pass]->SetUniformValue<glm::mat4>(name.c_str(), mat);
}

void Material::Bind(Ref<MaterialPass> pass, Transform* transform, const std::vector<VertexAttribute>& attributes)
{
	_hasError = pass->CompileShader(_state.shaderPath, attributes);
	SetTexture("linda_PrefilterSpecCube", Environment::GetPrefilterMap());
	SetTexture("linda_IrradianceCube", Environment::GetIrradianceMap());
	SetTexture("linda_BRDFLUT", Environment::GetBRDFLUTMap());
	pass->Bind(transform);
}

void Material::Bind(int pass, Transform* transform, const std::vector<VertexAttribute>& attributes)
{
	Bind(_passes[pass], transform, attributes);
}

IMPLEMENT_SETMATERIALUNIFORM(int)
IMPLEMENT_SETMATERIALUNIFORM(float)
IMPLEMENT_SETMATERIALUNIFORM(glm::vec4)
IMPLEMENT_SETMATERIALUNIFORM(glm::ivec4)
IMPLEMENT_SETMATERIALUNIFORM(const char*)

IMPLEMENT_SETMATERIALUNIFORM(glm::mat2)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat3)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat4)

IMPLEMENT_SETMATERIALUNIFORM(int*)
IMPLEMENT_SETMATERIALUNIFORM(float*)
IMPLEMENT_SETMATERIALUNIFORM(glm::vec4*)
IMPLEMENT_SETMATERIALUNIFORM(glm::ivec4*)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat2*)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat3*)
IMPLEMENT_SETMATERIALUNIFORM(glm::mat4*)
IMPLEMENT_SETMATERIALUNIFORM(Ref<Texture>)

void Material::AddKeyword(std::string key)
{
	for (auto& pass : _passes)
	{
		pass->AddKeyword(key);
	}
}

bool Material::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "FilePath" << YAML::Value << _state.materialPath;
	out << YAML::EndMap;

	return true;
}

bool Material::Deserialize(YAML::Node& node)
{
	_state.materialPath = node["FilePath"].as<std::string>();
	return true;
}

void Material::OnImguiRender()
{
	ImGui::PushID(this);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	const ImGuiStyle& style = ImGui::GetStyle();
	const float lineH = ImGui::GetFrameHeight();
	const float iconSize = lineH * 2.0f + style.ItemSpacing.y;

	Ref<Texture> iconTex = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Material.png");
	const ImTextureID iconId = iconTex ? (ImTextureID)(uintptr_t)iconTex->nativeColorID : (ImTextureID)0;

	if (ImGui::BeginTable("MaterialImGuiHeader", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("icon", ImGuiTableColumnFlags_WidthFixed, iconSize * 0.8f);
		ImGui::TableSetupColumn("fields", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		if (iconId)
			ImGui::Image(iconId, ImVec2(iconSize * 0.8f, iconSize), ImVec2(-0.13f, 1.25f), ImVec2(1.13f, -0.25f));
		else
			ImGui::Dummy(ImVec2(iconSize, iconSize));

		ImGui::TableSetColumnIndex(1);
		ImGui::TextUnformatted(Path::GetFileNameNoExtension(_state.materialPath).c_str());

		ImVec4 bgColor = style.Colors[ImGuiCol_FrameBg];
		ImVec4 bgActive = style.Colors[ImGuiCol_FrameBgActive];
		ImGui::PushStyleColor(ImGuiCol_Header, bgColor);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, bgColor);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, bgActive);

		ImGui::SetNextItemWidth(-FLT_MIN);
		const ImVec2 availSize = ImVec2(ImGui::GetContentRegionAvail().x, lineH);
		if (ImGui::Selectable("##shaderDrop", false, ImGuiSelectableFlags_Highlight, availSize))
		{
		}

		ImGui::PopStyleColor(3);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILENODE_DRAG"))
			{
				LindaEditor::FileNode* node = *(LindaEditor::FileNode**)payload->Data;
				if (node && node->type == LindaEditor::FileType::Shader)
				{
					bool isSkin = false;
					for (auto& pass : _passes)
					{
						for (auto& kw : pass->_state.keywords)
						{
							if (kw == "_Skin_Vertex_")
							{
								isSkin = true;
								break;
							}
						}
						if (isSkin)
							break;
					}

					Ref<Material> srcMat = MaterialManager::GetMaterialByShader(node->path, isSkin);
					if (srcMat)
					{
						std::string savedMatPath = _state.materialPath;
						_state = srcMat->_state;
						_state.materialPath = savedMatPath;
						_passes = std::move(srcMat->_passes);
						_hasError = false;
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImVec2 itemMin = ImGui::GetItemRectMin();
		ImVec2 itemMax = ImGui::GetItemRectMax();
		const float itemHeight = itemMax.y - itemMin.y;
		float contentY = itemMin.y + (itemHeight - 18.0f) * 0.5f; // 18是图标高度
		float x = itemMin.x + style.FramePadding.x;
		ImGui::SetCursorScreenPos(ImVec2(x, contentY));
		Ref<Texture> matIconTex = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Shader.png");
		const ImTextureID matIconId = matIconTex ? (ImTextureID)(uintptr_t)matIconTex->nativeColorID : (ImTextureID)0;
		ImGui::Image(matIconId, ImVec2(18, 18), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine(0, 6);

		std::string shaderPathStr = _state.shaderPath;
		std::string shaderLabel = shaderPathStr.empty() ? std::string("(Drag shader)") : Path::GetFileNameNoExtension(shaderPathStr);
		const ImVec2 textSize = ImGui::CalcTextSize(shaderLabel.c_str());
		const float textY = itemMin.y + (itemHeight - textSize.y) * 0.5f;
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, textY));

		if (shaderPathStr.empty())
			ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]);
		ImGui::TextUnformatted(shaderLabel.c_str());
		if (shaderPathStr.empty())
			ImGui::PopStyleColor();

		ImGui::EndTable();
	}

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	for (auto& pass : _passes)
	{
		ImGui::TextUnformatted(("LightMode : " + pass->GetLightMode()).c_str());

		std::vector<const char*> labels;
		for (auto& uniform : pass->_state.uniformNameMap)
		{
			switch (uniform.second->dataType)
			{
			case UniformType::INT:
			case UniformType::INT4:
			case UniformType::FLOAT:
			case UniformType::FLOAT4:
				labels.push_back(uniform.first.c_str());
				break;
			}
		}
		float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth(labels);

		for (auto& uniform : pass->_state.orderVisible)
		{
			switch (uniform->dataType)
			{
			case UniformType::TEXTURE:
			{
				const ImGuiStyle& style = ImGui::GetStyle();
				const float lineH = ImGui::GetFrameHeight();
				const float iconSize = lineH * 2.85f;

				// 获取图标纹理
				Ref<TextureUniformData> tud = DynamicCastRef(TextureUniformData, uniform);
				Ref<Texture> iconTex = TextureManager::GetTexture(tud->value);

				uint64_t iconId = 0;
				if (iconTex->type == TextureType::Cube)
				{
					static Ref<Material> material = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/CubemapVisible.shader");
					material->SetUniformValue("skybox", iconTex);
					Ref<RenderTexture> rt = TextureDriver::RenderMaterialBall(material);
					iconId = rt->nativeIDs[0];
				}
				else
					iconId = iconTex->nativeColorID;


				static float posX = 0.0f, posY = 0.0f;
				static float scaleX = 1.0f, scaleY = 1.0f;

				if (ImGui::BeginTable("ThreeLineRow", 2, ImGuiTableFlags_SizingFixedFit))
				{
					// 列设置：左侧内容自适应，右侧图标固定宽度
					ImGui::TableSetupColumn("content", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("icon", ImGuiTableColumnFlags_WidthFixed, iconSize);
					ImGui::TableNextRow();

					// === 第0列：左侧三行内容 ===
					ImGui::TableSetColumnIndex(0);

					float frontWidth = ImGui::CalcTextSize("    Offset     ").x;
					float availableWidth = ImGui::GetContentRegionAvail().x;
					float spacing = ImGui::GetStyle().ItemSpacing.x;
					float componentWidth = (availableWidth - frontWidth - spacing) / 2.0f;
					float inputWidth = componentWidth - ImGui::CalcTextSize("X").x - spacing;

					// 第一行：标题
					ImGui::TextUnformatted(uniform->name.c_str());

					auto itr = pass->_state.uniformNameMap.find(uniform->name + "_ST");
					if (itr != pass->_state.uniformNameMap.end())
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.26f, 0.26f, 0.26f, 0.0f }); // 透明背景
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.36f, 0.36f, 0.36f, 0.0f });
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.56f, 0.56f, 0.56f, 0.0f });
						
						glm::vec4& st = DynamicCastRef(Float4UniformData, (*itr).second)->value;

						// 第二行：Tiling x [ ] y [ ]
						ImGui::AlignTextToFramePadding();
						ImGui::TextUnformatted("    Tiling     ");
						ImGui::SameLine();
						ImGui::SetCursorPosX(frontWidth);
						ImGui::Text("X"); 
						ImGui::SameLine();
						ImGui::SetNextItemWidth(inputWidth);
						ImGui::DragFloat("##scaleX", &st.x, 0.01f);
						ImGui::SameLine();
						ImGui::Text("Y"); ImGui::SameLine();
						ImGui::SetNextItemWidth(inputWidth);
						ImGui::DragFloat("##scaleY", &st.y, 0.01f);

						// 第三行：Offset x [ ] y [ ]
						ImGui::AlignTextToFramePadding();
						ImGui::TextUnformatted("    Offset     ");
						ImGui::SameLine();
						ImGui::SetCursorPosX(frontWidth);
						ImGui::Text("X"); 
						ImGui::SameLine();
						ImGui::SetNextItemWidth(inputWidth);
						ImGui::DragFloat("##offsetX", &st.z, 0.01f);
						ImGui::SameLine();
						ImGui::Text("Y"); 
						ImGui::SameLine();
						ImGui::SetNextItemWidth(inputWidth);
						ImGui::DragFloat("##offsetY", &st.w, 0.01f);

						ImGui::PopStyleColor(3);
					}

					// === 第1列：右侧图标（垂直居中）===
					ImGui::TableSetColumnIndex(1);

					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
					if (iconId)
					{
						if (ImGui::ImageButton(uniform->name.c_str(), iconId, ImVec2(iconSize - 8, iconSize - 8)))
						{

						}
					}
					else
						ImGui::Dummy(ImVec2(iconSize, iconSize));
					ImGui::PopStyleVar(2);

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILENODE_DRAG"))
						{
							LindaEditor::FileNode* node = *(LindaEditor::FileNode**)payload->Data;
							if (node && node->type == LindaEditor::FileType::Texture)
							{
								DynamicCastRef(TextureUniformData, uniform)->value = node->path;
							}
						}
						ImGui::EndDragDropTarget();
					}

					ImGui::EndTable();
				}
				break;
			}
			case UniformType::INT:
				GUILayoutEditor::DragInt(uniform->name, &(DynamicCastRef(IntUniformData, uniform)->value), nullptr, 1.0f, 0, 0, firstWidth);
				break;
			case UniformType::INT4:
				GUILayoutEditor::DragInt4(uniform->name, (int*)&(DynamicCastRef(Int4UniformData, uniform)->value), nullptr, 1.0f, 0, 0, firstWidth);
				break;
			case UniformType::FLOAT:
				GUILayoutEditor::DragFloat(uniform->name, &(DynamicCastRef(FloatUniformData, uniform)->value), nullptr, 0.001f, 0.0f, 0.0f, firstWidth);
				break;
			case UniformType::FLOAT4:
			{
				auto float4Ptr = DynamicCastRef(Float4UniformData, uniform);
				if (float4Ptr->isColor)
				{
					GUILayoutEditor::ColorEdit4(uniform->name, (float*)&(float4Ptr->value), nullptr, firstWidth, float4Ptr->isHDR);
				}
				else
				{
					if (uniform->name.find("_TexelSize") == std::string::npos && uniform->name.find("_ST") == std::string::npos)
						GUILayoutEditor::DragFloat4(uniform->name, (float*)&(float4Ptr->value), nullptr, 0.001f, 0.0f, 0.0f, firstWidth);
				}
			}
			break;
			}

		}

		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
		ImGui::PopStyleColor(1);
	}

	GUILayoutEditor::DragInt("RenderQueue        ", &_state.renderQueue, nullptr, 1.0f, 0, 5000);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
	ImGui::PopStyleColor(1);

	ImGui::PopID();
}

bool Material::CanRender(std::string& lightMode, int minQueue, int maxQueue)
{
	return false == _hasError && _state.renderQueue >= minQueue && _state.renderQueue <= maxQueue && HasLightMode(lightMode);
}

bool Material::HasLightMode(std::string& lightMode)
{
	for (auto& pass : _passes)
	{
		if (pass->_state.lightMode == lightMode)
			return true;
	}

	if (_state.hasFallback)
	{
		if (nullptr != MaterialManager::GetDefaultMaterialPass(lightMode.c_str()))
			return true;
	}
	return false;
}

std::vector<Ref<MaterialPass>> Material::GetPassByLightMode(std::string& lightMode)
{
	std::vector<Ref<MaterialPass>> go;
	for (auto& pass : _passes)
	{
		if (pass->_state.lightMode == lightMode)
			go.push_back(pass);
	}
	if (go.size() <= 0 && _state.hasFallback)
	{
		Ref<MaterialPass> defaultPass = MaterialManager::GetDefaultMaterialPass(lightMode.c_str());
		if (nullptr != defaultPass)
			go.push_back(defaultPass);
	}
	return go;
}
