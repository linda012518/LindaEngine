#include "Renderer.h"
#include "YamlSerializer.h"

using namespace LindaEngine;

Renderer::Renderer(Entity& entity, bool enable) : Component(entity, enable)
{
}

Renderer::~Renderer()
{
}

MeshRenderer::MeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	//out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
	//out << YAML::Key << "Value" << YAML::Value << pointer->value;
	//out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
	out << YAML::EndMap;
}

bool MeshRenderer::Deserialize()
{
	return true;
}

SkinMeshRenderer::SkinMeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{

}

SkinMeshRenderer::~SkinMeshRenderer()
{
}

void SkinMeshRenderer::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	//out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
	//out << YAML::Key << "Value" << YAML::Value << pointer->value;
	//out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
	out << YAML::EndMap;
}

bool SkinMeshRenderer::Deserialize()
{
	return true;
}
