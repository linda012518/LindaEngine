#include "YamlSerializer.h"
#include "GraphicsContext.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;

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


