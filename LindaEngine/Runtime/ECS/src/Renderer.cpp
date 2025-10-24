#include "Renderer.h"
#include "YamlSerializer.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshManager.h"
#include "MaterialManager.h"

using namespace LindaEngine;

DYNAMIC_CREATE(MeshRenderer)
DYNAMIC_CREATE(SkinMeshRenderer)

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
	out << YAML::Key << "Mesh";
	_mesh->Serialize();
	out << YAML::Key << "Material";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& mat : _materialList)
	{
		mat->Serialize();
	}
	out << YAML::EndSeq;
	return true;
}

bool Renderer::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();

	auto mesh = node["Mesh"];
	auto materials = node["Material"];
	
	_mesh = MeshManager::GetMesh(mesh["FilePath"].as<std::string>().c_str());
	_mesh->Deserialize(mesh);

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

void Renderer::Render(std::string& lightMode)
{
	int index = 0;
	for (auto& material : _materialList)
	{
		index++;
		if (material->Bind(lightMode) == false)
			continue;
		_mesh->Draw(index - 1);
	}
}

/////////////////////////////////////////////////////////////////////

MeshRenderer::MeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
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

