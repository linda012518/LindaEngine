#include "YamlSerializer.h"
#include "GraphicsContext.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;

void YamlSerializer::Serialize(const char* path)
{
}

bool YamlSerializer::DeSerialize(const char* path)
{
	return true;
}

void YamlSerializer::SerializeMaterial(const char* path)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Material";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "FileName" << YAML::Value << "Name";
	out << YAML::Key << "ShaderPath" << YAML::Value << "Path";
	out << YAML::Key << "ShadowCast" << YAML::Value << "false";
	out << YAML::Key << "ReceiveShadow" << YAML::Value << "true";

	out << YAML::Key << "MaterialPasses";
	out << YAML::Value << YAML::BeginSeq;
	SerializeMaterialPass(out);
	SerializeMaterialPass(out);
	out << YAML::EndSeq;

	out << YAML::EndMap;
	out << YAML::EndMap;

	try
	{
		std::ofstream fout(path);
		fout << out.c_str();
	}
	catch (const std::exception&)
	{
		std::cout << "YamlSerializer::SerializeMaterial Error" << path << "\n" << std::endl;
	}
}

bool YamlSerializer::DeSerializeMaterial(const char* path)
{
	YAML::Node data;
	try
	{
		data = YAML::LoadFile(path);
	}
	catch (const std::exception&)
	{
		return false;
	}

	if (!data["Material"])
		return false;

	auto material = data["Material"];

	std::string FileName = material["FileName"].as<std::string>();
	std::string ShaderPath = material["ShaderPath"].as<std::string>();
	bool ShadowCast = material["ShadowCast"].as<bool>();
	bool ReceiveShadow = material["ReceiveShadow"].as<bool>();

	auto passes = material["MaterialPasses"];
	for (auto pass : passes)
	{
		std::string PassName = pass["PassName"].as<std::string>();
		std::string RenderType = pass["RenderType"].as<std::string>();
		int RenderQueue = pass["RenderQueue"].as<int>();
		auto Uniforms = pass["Uniforms"];
		float UniformName1 = Uniforms["UniformName1"].as<float>();
		std::string UniformName2 = Uniforms["UniformName2"].as<std::string>();
		auto Keywords = pass["Keywords"];
		for (std::size_t i = 0; i < Keywords.size(); i++) {
			std::string go = Keywords[i].as<std::string>();
			int a = 0;
		}
		auto RenderState = pass["RenderState"];
		std::string ColorMask = RenderState["ColorMask"].as<std::string>();
		std::string CullFaceFunc = RenderState["CullFaceFunc"].as<std::string>();
		auto DepthState = RenderState["DepthState"];
		bool DepthTest = DepthState["DepthTest"].as<bool>();
		auto PolygonModeState = RenderState["PolygonModeState"];
		std::string PolygonMode = PolygonModeState["PolygonMode"].as<std::string>();
		auto StencilState = RenderState["StencilState"];
		bool StencilTest = StencilState["StencilTest"].as<bool>();
		auto BlendState = RenderState["BlendState"];
		bool EnableBlend = BlendState["EnableBlend"].as<bool>();
		int b = 0;
	}

	return true;
}

void YamlSerializer::SerializeGraphicsConfig(const char* path)
{
	GraphicsConfig& config = GraphicsContext::graphicsConfig;

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "GraphicsConfig";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "redBits" << YAML::Value << config.redBits;
	out << YAML::Key << "greenBits" << YAML::Value << config.greenBits;
	out << YAML::Key << "blueBits" << YAML::Value << config.blueBits;
	out << YAML::Key << "alphaBits" << YAML::Value << config.alphaBits;
	out << YAML::Key << "depthBits" << YAML::Value << config.depthBits;
	out << YAML::Key << "stencilBits" << YAML::Value << config.stencilBits;
	out << YAML::Key << "msaaSamples" << YAML::Value << config.msaaSamples;
	out << YAML::Key << "screenWidth" << YAML::Value << config.screenWidth;
	out << YAML::Key << "screenHeight" << YAML::Value << config.screenHeight;
	out << YAML::Key << "appName" << YAML::Value << config.appName;
	out << YAML::Key << "graphicsAPI" << YAML::Value << static_cast<int>(config.graphicsAPI);
	out << YAML::Key << "platformOS" << YAML::Value << static_cast<int>(config.platformOS);
	out << YAML::EndMap;
	out << YAML::EndMap;

	try
	{
		std::ofstream fout(path);
		fout << out.c_str();
	}
	catch (const std::exception&)
	{
		std::cout << "YamlSerializer::SerializeGfxConfiguration Error" << path << "\n" << std::endl;
	}

}

bool YamlSerializer::DeSerializeGraphicsConfig(const char* path)
{
	YAML::Node data;
	try
	{
		data = YAML::LoadFile(path);
	}
	catch (const std::exception&)
	{
		return false;
	}

	data = data["GraphicsConfig"];
	if (!data)
		return false;

	GraphicsConfig& config = GraphicsContext::graphicsConfig;

	config.redBits = data["redBits"].as<uint32_t>();
	config.greenBits = data["greenBits"].as<uint32_t>();
	config.blueBits = data["blueBits"].as<uint32_t>();
	config.alphaBits = data["alphaBits"].as<uint32_t>();
	config.depthBits = data["depthBits"].as<uint32_t>();
	config.stencilBits = data["stencilBits"].as<uint32_t>();
	config.msaaSamples = data["msaaSamples"].as<uint32_t>();
	config.screenWidth = data["screenWidth"].as<uint32_t>();
	config.screenHeight = data["screenHeight"].as<uint32_t>();
	std::string appName = data["appName"].as<std::string>();
	config.appName = appName.c_str();
	config.graphicsAPI = static_cast<GraphicsDriverAPI>(data["graphicsAPI"].as<int>());
	config.platformOS = static_cast<Platform>(data["platformOS"].as<int>());

	return true;
}

void YamlSerializer::SerializeMaterialPass(YAML::Emitter& out)
{
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "PassName" << YAML::Value << "Color";
	out << YAML::Key << "RenderType" << YAML::Value << "Opaque";
	out << YAML::Key << "RenderQueue" << YAML::Value << "2000";

	out << YAML::Key << "Uniforms";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "UniformName1" << YAML::Value << 66.6f;
	out << YAML::Key << "UniformName2" << YAML::Value << "aaa";
	out << YAML::EndMap;

	out << YAML::Key << "Keywords";
	out << YAML::Value << YAML::BeginSeq;
	out << "EnableLogDepth" << "NORMAL_MAP";
	out << YAML::EndSeq;

	out << YAML::Key << "RenderState";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "ColorMask" << YAML::Value << "RGBA";
	out << YAML::Key << "CullFaceFunc" << YAML::Value << "BACK";

	out << YAML::Key << "DepthState";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "DepthTest" << YAML::Value << false;
	out << YAML::EndMap;

	out << YAML::Key << "PolygonModeState";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "PolygonMode" << YAML::Value << "FILL";
	out << YAML::EndMap;

	out << YAML::Key << "StencilState";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "StencilTest" << YAML::Value << false;
	out << YAML::EndMap;

	out << YAML::Key << "BlendState";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "EnableBlend" << YAML::Value << false;
	out << YAML::EndMap;

	out << YAML::EndMap;
	out << YAML::EndMap;
}
