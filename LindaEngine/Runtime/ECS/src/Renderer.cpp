#include "Renderer.h"
#include "YamlSerializer.h"

using namespace LindaEngine;

DYNAMIC_CREATE(MeshRenderer)
DYNAMIC_CREATE(SkinMeshRenderer)

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

bool MeshRenderer::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	//out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
	//out << YAML::Key << "Value" << YAML::Value << pointer->value;
	//out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
	out << YAML::EndMap;

	return true;
}

bool MeshRenderer::Deserialize(YAML::Node& node)
{
	return true;
}

SkinMeshRenderer::SkinMeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{

}

SkinMeshRenderer::~SkinMeshRenderer()
{
}

bool SkinMeshRenderer::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	//out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
	//out << YAML::Key << "Value" << YAML::Value << pointer->value;
	//out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
	out << YAML::EndMap;

	return true;
}

bool SkinMeshRenderer::Deserialize(YAML::Node& node)
{
	return true;
}

