#include "TonemappingPostProcess.h"
#include "ClassImplement.inl"
#include "YamlSerializer.h"
#include "Graphic.h"
#include "Material.h"
#include "MaterialManager.h"

using namespace LindaEngine;

DYNAMIC_CREATE_CLASS(TonemappingPostProcess, PostProcessEffectRenderer)
COLLECT_POSTPROCESS(TonemappingPostProcess)

TonemappingPostProcess::TonemappingPostProcess()
{
	Initialize();
}

TonemappingPostProcess::~TonemappingPostProcess()
{
	Finalize();
}

void TonemappingPostProcess::Initialize()
{
	_simpleTonemapping = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/PostProcess/Tonemapping/SimpleTonemapping.shader");
}

void TonemappingPostProcess::Finalize()
{
	_simpleTonemapping = nullptr;
}

void TonemappingPostProcess::Render(Ref<RenderTexture> src, Ref<RenderTexture> dest)
{
	Graphic::Blit(src, dest, _simpleTonemapping);
}

bool TonemappingPostProcess::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Name" << YAML::Value << "TonemappingPostProcess";
	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::EndMap;

	return true;
}

bool TonemappingPostProcess::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	return true;
}