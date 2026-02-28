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

using namespace LindaEngine;

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
		_mesh = FBXManager::GetMesh(mesh["FilePath"].as<std::string>().c_str(), mesh["HashCode"].as<std::string>().c_str());
		_mesh->Deserialize(mesh);
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

void Renderer::SetMesh(Ref<Mesh> mesh)
{
	_mesh = mesh;
	_materialList.clear();
	FillDrawables();
}

void Renderer::AddMaterial(int index, Ref<Material> mat)
{
	_materialList.insert(_materialList.begin() + index, mat);

	if (_type == RenderComponentType::Skybox || _type == RenderComponentType::ScreenTriangle)
		return;
	_drawables[index]->material = mat;
}

void Renderer::TransformDirty()
{
	glm::vec3 min = _mesh->GetBoundingBox().min;
	glm::vec3 max = _mesh->GetBoundingBox().max;
	const glm::mat4& transform = _transform->GetLocalToWorldMat();
	_aabb = AABBBoundingBox::WorldSpaceAABB(transform, min, max);
}

bool Renderer::CanRender(int index, int minQueue, int maxQueue)
{
	if (_materialList.size() <= index)
		return false;

	return _materialList[index]->CanRender(Material::overrideLightMode, minQueue, maxQueue);
}

bool Renderer::HasError(int index)
{
	return _materialList[index]->HasError();
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

void Renderer::RenderSkybox()
{
	Material::overrideLightMode = "Skybox";
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
		mat->SetUniformValue<glm::mat4*>("bonesMatrices", _boneMatrices.data(), (int)_boneMatrices.size());
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


