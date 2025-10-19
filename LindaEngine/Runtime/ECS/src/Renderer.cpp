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

bool Renderer::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "enable" << YAML::Value << _enable;

	return true;
}

bool Renderer::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	return true;
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

	out << YAML::EndMap;

	return true;
}

bool SkinMeshRenderer::Deserialize(YAML::Node& node)
{
	Renderer::Deserialize(node);
	return true;
}

