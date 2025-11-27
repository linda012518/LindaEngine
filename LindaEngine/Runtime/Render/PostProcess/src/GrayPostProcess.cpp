#include "GrayPostProcess.h"
#include "YamlSerializer.h"
#include "Graphic.h"
#include "Material.h"
#include "MaterialManager.h"
#include "ClassImplement.inl"

using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(GrayPostProcess, PostProcessEffectRenderer)

GrayPostProcess::GrayPostProcess()
{
	Initialize();
}

GrayPostProcess::~GrayPostProcess()
{
	Finalize();
}

void GrayPostProcess::Initialize()
{
	_grayMaterial = MaterialManager::GetMaterialByShader("Assets/Shaders/gray.shader");
}

void GrayPostProcess::Finalize()
{
	_grayMaterial = nullptr;
}

void GrayPostProcess::Render(Ref<RenderTexture> src, Ref<RenderTexture> dest)
{
	Graphic::Blit(src, dest, _grayMaterial);
}

bool GrayPostProcess::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Name" << YAML::Value << "GrayPostProcess";
	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::EndMap;

	return true;
}

bool GrayPostProcess::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	return true;
}