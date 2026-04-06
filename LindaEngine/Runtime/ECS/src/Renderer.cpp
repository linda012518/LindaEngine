#include "Renderer.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "YamlSerializer.h"
#include "Material.h"
#include "FBXManager.h"
#include "MaterialManager.h"
#include "ComponentImplement.inl"
#include "Entity.h"
#include "RendererSystem.h"
#include "Drawable.h"
#include "Mathf.h"
#include "ShaderBuiltInUniform.h"
#include "Environment.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Path.h"
#include "ContentBrowserPanelEditor.h"
#include "GUILayoutEditor.h"

#include "imgui/imgui.h"

using namespace LindaEngine;
using namespace LindaEditor;

DYNAMIC_CREATE(MeshRenderer)
DYNAMIC_CREATE(SkinMeshRenderer)

Renderer::Renderer(Entity& entity, bool enable) : Component(entity, enable)
{
	RendererSystem::Add(this);
}

Renderer::~Renderer()
{
	RendererSystem::Remove(this);
}

bool Renderer::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::Key << "ShadowCast" << YAML::Value << _shadowCast;
	out << YAML::Key << "ReceiveShadow" << YAML::Value << _receiveShadow;
	if (nullptr != _mesh)
	{
		out << YAML::Key << "Mesh";
		_mesh->Serialize();
	}
	if (_materialList.size() > 0)
	{
		out << YAML::Key << "Material";
		out << YAML::Value << YAML::BeginSeq;
		for (auto& mat : _materialList)
		{
			if (nullptr == mat)
				continue;
			mat->Serialize();
		}
		out << YAML::EndSeq;
	}

	return true;
}

bool Renderer::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	_shadowCast = node["ShadowCast"].as<bool>();
	_receiveShadow = node["ReceiveShadow"].as<bool>();

	auto mesh = node["Mesh"];
	auto materials = node["Material"];
	
	if (mesh)
	{
		Ref<Mesh> meshPtr = FBXManager::GetMesh(mesh["FilePath"].as<std::string>().c_str(), mesh["HashCode"].as<std::string>().c_str());
		meshPtr->Deserialize(mesh);
		SetMesh(meshPtr);
	}

	bool isSkin = _type == RenderComponentType::SkinMesh;
	int index = 0;
	for (auto mat : materials)
	{
		Ref<Material> pointer = MaterialManager::GetMaterial(mat["FilePath"].as<std::string>().c_str(), isSkin);
		pointer->Deserialize(mat);
		AddMaterial(index++, pointer);
	}

	FillDrawables();
	return true;
}

void Renderer::OnImguiRender()
{
	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "Shadow Cast", "Receive Shadow", "Mesh", "Material 99" });

	GUILayoutEditor::Checkbox("Shadow Cast", &_shadowCast, [&]() {}, firstWidth);
	GUILayoutEditor::Checkbox("Receive Shadow", &_receiveShadow, [&]() {}, firstWidth);
	GUILayoutEditor::TextDescription("Mesh", _mesh ? _mesh->GetPath().c_str() : "None ???", !_mesh, firstWidth);

	if (nullptr == _mesh || _mesh->GetAllMeshData().size() <= 0)
		return;

	size_t matSlotCount = _mesh->GetAllMeshData().size();

	Ref<Texture> matIconTex = TextureManager::GetTextureDirect("BuiltInAssets/Icons/Material.png");
	const ImTextureID matIconId = matIconTex ? (ImTextureID)(uintptr_t)matIconTex->nativeColorID : (ImTextureID)0;

	if (ImGui::BeginTable("MaterialsTable", 2, ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);

		for (size_t i = 0; i < matSlotCount; ++i)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Material %zu", i);
			ImGui::TableSetColumnIndex(1);
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::PushID((int)i);

			const bool hasMaterial = (i < _materialList.size() && _materialList[i]);
			std::string display = hasMaterial
				? Path::GetFileNameNoExtension(_materialList[i]->GetPath())
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
						const bool isSkin = (_type == RenderComponentType::SkinMesh);
						AddMaterial((int)i, MaterialManager::GetMaterial(node->path, isSkin));
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (hasMaterial && ImGui::BeginPopupContextItem("##ClearMaterial"))
			{
				if (ImGui::MenuItem("Clear Material"))
				{
					//RemoveMaterial((int)i);
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

			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}

void Renderer::SetMesh(Ref<Mesh> mesh)
{
	_mesh = mesh;
	_materialList.clear();
	FillDrawables();
}

void Renderer::AddMaterial(int index, Ref<Material> mat)
{
	if (index < _materialList.size())
		_materialList[index] = mat;
	else
		_materialList.push_back(mat);

	if (_type == RenderComponentType::Skybox || _type == RenderComponentType::ScreenTriangle)
		return;
	_drawables[index]->material = mat;
}

void Renderer::TransformDirty()
{
	if (nullptr == _mesh)
		return;
	glm::vec3 min = _mesh->GetBoundingBox().min;
	glm::vec3 max = _mesh->GetBoundingBox().max;
	const glm::mat4& transform = _transform->GetLocalToWorldMat();
	_aabb = AABBBoundingBox::WorldSpaceAABB(transform, min, max);
}

bool Renderer::CanRender(int index, int minQueue, int maxQueue)
{
	if (_materialList.size() <= (size_t)index || !_materialList[(size_t)index])
		return false;

	return _materialList[(size_t)index]->CanRender(Material::overrideLightMode, minQueue, maxQueue);
}

bool Renderer::HasError(int index)
{
	if (_materialList.size() <= (size_t)index || !_materialList[(size_t)index])
		return false;
	return _materialList[(size_t)index]->HasError();
}

bool Renderer::InLayerMask(int layer)
{
	if (-1 == layer)
		return false;
	if (0 == layer)
		return true;
	return _entity.GetLayer() & layer;
}

Drawable& Renderer::GetSkyboxRenderer()
{
	static bool isLoaded = false;
	static Drawable drawable;
	if (isLoaded == false)
	{
		drawable.meshData = FBXManager::GetSkybox()->GetMeshData();
		drawable.transform = nullptr;
		drawable.material = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/SkyboxCubemap.shader");
		isLoaded = true;
	}
	return drawable;
}

void Renderer::SetSkyboxMaterial(Ref<Material> material)
{
	GetSkyboxRenderer().material = material;
}

Ref<Material> Renderer::GetSkyboxMaterial()
{
	return GetSkyboxRenderer().material;
}

void Renderer::RenderSkybox(glm::mat4& project)
{
	Material::overrideLightMode = "Skybox";
	GetSkyboxMaterial()->SetUniformValue("skybox", Environment::GetCubemap());
	GetSkyboxMaterial()->SetUniformValue(ShaderBuiltInUniform::linda_SkyboxMatrix_P.c_str(), project);
	GetSkyboxRenderer().Draw();
}

Drawable& Renderer::GetBoundingBoxRenderer()
{
	static bool isLoaded = false;
	static Drawable drawable;
	if (isLoaded == false)
	{
		drawable.meshData = FBXManager::GetBoundingBox()->GetMeshData();
		static Entity root("BoundingBox-Root"); root.SetDontDestory(true);
		static Entity child("BoundingBox-Child"); child.SetDontDestory(true);
		child.GetTransform()->SetParent(root.GetTransform());
		drawable.transform = child.GetTransform();
		drawable.material = MaterialManager::GetDefaultMaterial("BuiltInAssets/Shaders/BoundingBox.shader");
		isLoaded = true;
	}
	return drawable;
}

void Renderer::RenderBoundingBox()
{
	if (nullptr == _mesh)
		return;
	Material::overrideLightMode = "Adjunct";
	Drawable& drawable = GetBoundingBoxRenderer();
	drawable.transform->SetLocalPosition(_transform->GetWorldScale() * _mesh->GetBoundingBox().center);
	drawable.transform->SetWorldScale(_transform->GetWorldScale() * _mesh->GetBoundingBox().size);
	drawable.transform->Tick();
	Transform* parent = (Transform*)drawable.transform->GetParent();
	parent->SetWorldPosition(_transform->GetWorldPosition());
	parent->SetWorldRotation(_transform->GetWorldRotation());
	parent->Tick();
	drawable.Draw();
}

void Renderer::FillDrawables()
{
	if (_type == RenderComponentType::Skybox || _type == RenderComponentType::ScreenTriangle)
		return;

	_drawables.clear();

	int index = 0;
	for (auto& data : _mesh->GetAllMeshData())
	{
		Ref<Drawable> da = CreateRef<Drawable>();
		da->meshData = &data;
		da->transform = _transform;
		if (_materialList.size() > index)
			da->material = _materialList[index];
		else
			da->material = nullptr;
		index++;
		_drawables.push_back(da);
	}
}

/////////////////////////////////////////////////////////////////////

MeshRenderer::MeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
	_type = RenderComponentType::Mesh;
}

MeshRenderer::~MeshRenderer()
{

}

bool MeshRenderer::Serialize()
{
	Renderer::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;
	out << YAML::Key << "Name" << YAML::Value << "MeshRenderer";
	out << YAML::EndMap;

	return true;
}

bool MeshRenderer::Deserialize(YAML::Node& node)
{
	Renderer::Deserialize(node);
	return true;
}

/////////////////////////////////////////////////////////////////////

SkinMeshRenderer::SkinMeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
	_type = RenderComponentType::SkinMesh;
}

SkinMeshRenderer::~SkinMeshRenderer()
{
}

void SkinMeshRenderer::Tick()
{
	int size1 = (int)_bonesData.size();
	int size2 = (int)_bones.size();
	if (size1 <= 0 || size2 <= 0 || size1 != size2)
		return;

	int maxBoneId = -1;
	for (int n = 0; n < size1; n++)
		maxBoneId = std::max(maxBoneId, _bonesData[n].id);
	if (maxBoneId < 0)
		return;

	const glm::mat4& meshWorldToLocal = _transform->GetWorldToLocalMat();

	_boneMatrices.assign((size_t)maxBoneId + 1, glm::mat4(1.0f));
	for (int n = 0; n < size1; n++)
	{
		Transform* bone = _bones[n];
		if (bone == nullptr)
			continue;

		int boneId = _bonesData[n].id;
		if (boneId < 0 || boneId > maxBoneId)
			continue;

		// Convert bone world transform into skinned-mesh local space,
		// then apply inverse bind pose (offset) to get final skin matrix.
		_boneMatrices[(size_t)boneId] = meshWorldToLocal * bone->GetLocalToWorldMat() * _bonesData[n].offset;
	}
	for (auto& mat : _materialList)
	{
		mat->SetUniformValue<glm::mat4*>(ShaderBuiltInUniform::linda_BonesMatrices.c_str(), _boneMatrices.data(), (int)_boneMatrices.size());
	}
}

bool SkinMeshRenderer::Serialize()
{
	Renderer::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;
	out << YAML::Key << "Name" << YAML::Value << "SkinMeshRenderer";
	out << YAML::Key << "RootBone" << YAML::Value << _rootBone->GetEntity().GetUUID();

	out << YAML::Key << "Bones";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& bone : _bones)
	{
		if (nullptr == bone)
			continue;
		out << bone->GetEntity().GetUUID();
	}
	out << YAML::EndSeq;

	out << YAML::EndMap;

	return true;
}

bool SkinMeshRenderer::Deserialize(YAML::Node& node)
{
	Renderer::Deserialize(node);
	std::string rootBoneID = node["RootBone"].as<std::string>();
	_rootBone = TransformSystem::Get(rootBoneID);
	auto bones = node["Bones"];
	if (bones)
	{
		for (std::size_t i = 0; i < bones.size(); i++)
		{
			std::string uuid = bones[i].as<std::string>();
			auto ptr = TransformSystem::Get(uuid);
			if (nullptr == ptr)
				continue;
			_bones.push_back(ptr);
		}
	}

	if (nullptr != _mesh)
	{
		_bonesData = FBXManager::GetMeshBoneData(_mesh->GetPath(), _mesh->GetHashCode());
	}
	return true;
}

void SkinMeshRenderer::OnImguiRender()
{
	Renderer::OnImguiRender();

	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "Root Bone", "Bone Count" });

	if (ImGui::BeginTable("RootBoneTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Root Bone");
		ImGui::TableSetColumnIndex(1);
		if (_rootBone)
			ImGui::TextWrapped("%s", _rootBone->GetEntity().GetName().c_str());
		else
			ImGui::TextDisabled("(None)");
		ImGui::EndTable();
	}

	if (ImGui::BeginTable("BonesCountTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Bone Count");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%zu", _bones.size());
		ImGui::EndTable();
	}

	for (size_t i = 0; i < _bones.size(); ++i)
	{
		ImGui::PushID((int)i);
		if (ImGui::BeginTable("BoneSlot", 2, ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
			ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("[%zu]", i);
			ImGui::TableSetColumnIndex(1);
			if (_bones[i])
				ImGui::TextWrapped("%s", _bones[i]->GetEntity().GetName().c_str());
			else
				ImGui::TextDisabled("(None)");
			ImGui::EndTable();
		}
		ImGui::PopID();
	}
}

void SkinMeshRenderer::AddMaterial(int index, Ref<Material> mat)
{
	Renderer::AddMaterial(index, mat);
	mat->AddKeyword("_Skin_Vertex_");
}

void SkinMeshRenderer::SetBones(std::vector<Transform*> bones)
{
	_bones = bones;
}

std::vector<Transform*>& SkinMeshRenderer::GetBones()
{
	return _bones;
}

void SkinMeshRenderer::SetRootBone(Transform* root)
{
	_rootBone = root;
}

Transform* SkinMeshRenderer::GetRootBone()
{
	return _rootBone;
}

void SkinMeshRenderer::SetBonesData(std::vector<BoneData> data)
{
	_bonesData = data;
}

std::vector<BoneData>& SkinMeshRenderer::GetBonesData()
{
	return _bonesData;
}


