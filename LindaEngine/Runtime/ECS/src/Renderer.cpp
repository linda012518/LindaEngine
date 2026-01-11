#include "Renderer.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "Material.h"
#include "FBXManager.h"
#include "MaterialManager.h"
#include "ComponentImplement.inl"
#include "Entity.h"
#include "RendererSystem.h"
#include "Drawable.h"

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

	for (auto mat : materials)
	{
		Ref<Material> pointer = MaterialManager::GetMaterial(mat["FilePath"].as<std::string>().c_str());
		_materialList.push_back(pointer);
		pointer->Deserialize(mat);
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
	_aabb.min = _transform->GetLocalToWorldMat() * glm::vec4(_mesh->GetBoundingBox().min, 1.0f);
	_aabb.max = _transform->GetLocalToWorldMat() * glm::vec4(_mesh->GetBoundingBox().max, 1.0f);
	_aabb.CalculateCenterSize();
}

bool Renderer::CanRender(int index, int minQueue, int maxQueue)
{
	if (_materialList.size() <= index)
		return false;

	return _materialList[index]->CanRender(Material::overrideLightMode, minQueue, maxQueue);
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
	static Drawable drawable;
	drawable.meshData = FBXManager::GetSkybox()->GetMeshData();
	drawable.transform = nullptr;
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

Renderer* Renderer::GetBoundingBoxRenderer()
{
	return nullptr;
}

void Renderer::RenderBoundingBox(Renderer* src)
{
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

bool SkinMeshRenderer::Serialize()
{
	Renderer::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;
	out << YAML::Key << "Name" << YAML::Value << "SkinMeshRenderer";
	out << YAML::EndMap;

	return true;
}

bool SkinMeshRenderer::Deserialize(YAML::Node& node)
{
	Renderer::Deserialize(node);
	return true;
}


