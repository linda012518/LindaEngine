#include "Renderer.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "ComponentImplement.inl"
#include "Entity.h"

using namespace LindaEngine;

DYNAMIC_CREATE(MeshRenderer)
DYNAMIC_CREATE(SkinMeshRenderer)
DYNAMIC_CREATE(SkyboxRenderer)

Renderer::Renderer(Entity& entity, bool enable) : Component(entity, enable)
{
}

Renderer::~Renderer()
{
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
		_mesh = MeshManager::GetMesh(mesh["FilePath"].as<std::string>().c_str());
		_mesh->Deserialize(mesh);
	}

	for (auto mat : materials)
	{
		Ref<Material> pointer = MaterialManager::GetMaterial(mat["FilePath"].as<std::string>().c_str());
		_materialList.push_back(pointer);
		pointer->Deserialize(mat);
	}
	return true;
}

void Renderer::SetMesh(Ref<Mesh> mesh)
{
	_mesh = mesh;
}

void Renderer::AddMaterial(int index, Ref<Material> mat)
{
	_materialList.insert(_materialList.begin() + index, mat);
}

void Renderer::Render()
{
	int index = 0;
	for (auto& material : _materialList)
	{
		std::vector<Ref<MaterialPass>> go = material->GetPassByLightMode(Material::overrideLightMode);
		if (go.size() <= 0)
			continue;
		for (auto& pass : go)
		{
			material->Bind(pass, _transform, _mesh->GetMeshAttributes(index));
			_mesh->Draw(index);
		}
		index++;
	}
}

void Renderer::TransformDirty()
{
	_aabb.min = glm::vec4(_mesh->GetBoundingBox().min, 1.0f) * _transform->GetLocalToWorldMat();
	_aabb.max = glm::vec4(_mesh->GetBoundingBox().max, 1.0f) * _transform->GetLocalToWorldMat();
	_aabb.CalculateCenterSize();
}

Renderer* Renderer::GetSkyboxRenderer()
{
	static Entity entity("");
	static SkyboxRenderer renderer(entity);
	return &renderer;
}

void Renderer::SetSkyboxMaterial(Ref<Material> material)
{
	GetSkyboxRenderer()->GetMaterials().clear();
	GetSkyboxRenderer()->AddMaterial(0, material);
}

Ref<Material> Renderer::GetSkyboxMaterial()
{
	return GetSkyboxRenderer()->GetMaterials()[0];
}

void Renderer::RenderSkybox()
{
	Material::overrideLightMode = "Skybox";
	GetSkyboxRenderer()->Render();
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

/////////////////////////////////////////////////////////////////////

SkyboxRenderer::SkyboxRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
	_type = RenderComponentType::Skybox;
	_materialList.push_back(MaterialManager::GetSkybox());
	_mesh = MeshManager::GetSkybox();
	_shadowCast = false;
	_receiveShadow = false;
}

SkyboxRenderer::~SkyboxRenderer()
{
}

bool SkyboxRenderer::Serialize()
{
	Renderer::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;
	out << YAML::Key << "Name" << YAML::Value << "SkyboxRenderer";
	out << YAML::EndMap;

	return true;
}

bool SkyboxRenderer::Deserialize(YAML::Node& node)
{
	Renderer::Deserialize(node);
	return true;
}

