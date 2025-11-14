#include "YamlSerializer.h"
#include "Texture.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;

void YamlSerializer::SerializeTexture(const char* path)
{
	Ref<Texture> texture = Texture::overrideTexture;

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Texture";
	out << YAML::Value << YAML::BeginMap;
	Ref<Cubemap> cubemap = DynamicCastRef(Cubemap, texture);
	if (texture->type == TextureType::Cube && cubemap != nullptr)
	{
		out << YAML::Key << "left" << YAML::Value << cubemap->left;
		out << YAML::Key << "right" << YAML::Value << cubemap->right;
		out << YAML::Key << "top" << YAML::Value << cubemap->top;
		out << YAML::Key << "bottom" << YAML::Value << cubemap->bottom;
		out << YAML::Key << "front" << YAML::Value << cubemap->front;
		out << YAML::Key << "back" << YAML::Value << cubemap->back;
	}
	else
	{
		out << YAML::Key << "FilePath" << YAML::Value << texture->path;
	}
	out << YAML::Key << "type" << YAML::Value << static_cast<int>(texture->type);
	out << YAML::Key << "isGammaCorrection" << YAML::Value << texture->isGammaCorrection;
	out << YAML::Key << "mipmapCount" << YAML::Value << texture->mipmapCount;
	out << YAML::Key << "filter" << YAML::Value << static_cast<int>(texture->filter);
	out << YAML::Key << "warpU" << YAML::Value << static_cast<int>(texture->warpU);
	out << YAML::Key << "warpV" << YAML::Value << static_cast<int>(texture->warpV);
	out << YAML::Key << "warpW" << YAML::Value << static_cast<int>(texture->warpW);
	out << YAML::EndMap;
	out << YAML::EndMap;

	try
	{
		std::ofstream fout(path);
		fout << out.c_str();
	}
	catch (const std::exception&)
	{
		std::cout << "YamlSerializer::SerializeTexture Error" << path << "\n" << std::endl;
	}

}

Ref<Texture> YamlSerializer::DeSerializeTexture(const char* path)
{
	YAML::Node data;
	try
	{
		data = YAML::LoadFile(path);
	}
	catch (const std::exception&)
	{
		return nullptr;
	}

	data = data["Texture"];
	if (!data)
		return nullptr;

	TextureType type = static_cast<TextureType>(data["type"].as<int>());

	Ref<Texture> texture = nullptr;
	if (type == TextureType::Tex2D)
	{
		texture = CreateRef<Texture2D>();
		std::string go = data["FilePath"].as<std::string>();
		texture->path = go.c_str();
	}
	else if (type == TextureType::Cube)
	{
		Ref<Cubemap> cubemap = CreateRef<Cubemap>();
		texture = cubemap;

		std::string go = data["left"].as<std::string>();
		cubemap->left = go.c_str();
		std::string go1 = data["right"].as<std::string>();
		cubemap->right = go1.c_str();
		std::string go2 = data["top"].as<std::string>();
		cubemap->top = go2.c_str();
		std::string go3 = data["bottom"].as<std::string>();
		cubemap->bottom = go3.c_str();
		std::string go4 = data["front"].as<std::string>();
		cubemap->front = go4.c_str();
		std::string go5 = data["back"].as<std::string>();
		cubemap->back = go5.c_str();
	}

	texture->isGammaCorrection = data["isGammaCorrection"].as<bool>();
	texture->mipmapCount = data["mipmapCount"].as<int>();
	texture->filter = static_cast<FilterMode>(data["filter"].as<int>());
	texture->warpU = static_cast<TextureWrapMode>(data["warpU"].as<int>());
	texture->warpV = static_cast<TextureWrapMode>(data["warpV"].as<int>());
	texture->warpW = static_cast<TextureWrapMode>(data["warpW"].as<int>());

	return texture;

}
