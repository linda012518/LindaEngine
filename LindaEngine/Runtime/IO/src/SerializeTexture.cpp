#include "YamlSerializer.h"
#include "Texture.h"
#include "TextureManager.h"

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
	if (texture->type == TextureType::Cube && cubemap != nullptr && cubemap->srcType == CubemapSrcType::SixTexture)
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
	out << YAML::Key << "anisotropy" << YAML::Value << texture->anisotropy;
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

		if (!data["FilePath"])
		{
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
			cubemap->srcType = CubemapSrcType::SixTexture;
		}
		else
		{
			std::string go = data["FilePath"].as<std::string>();
			cubemap->path = go.c_str();
		}
	}

	texture->isGammaCorrection = data["isGammaCorrection"].as<bool>();
	texture->mipmapCount = data["mipmapCount"].as<int>();
	texture->anisotropy = data["anisotropy"].as<int>();
	texture->filter = static_cast<FilterMode>(data["filter"].as<int>());
	texture->warpU = static_cast<TextureWrapMode>(data["warpU"].as<int>());
	texture->warpV = static_cast<TextureWrapMode>(data["warpV"].as<int>());
	texture->warpW = static_cast<TextureWrapMode>(data["warpW"].as<int>());
	texture->nodePath = path;

	return texture;

}

void YamlSerializer::SerializeRenderTexture(const char* path)
{
	Ref<RenderTexture> texture = DynamicCastRef(RenderTexture, Texture::overrideTexture);
	if (nullptr == texture)
	{
		std::cout << "YamlSerializer::SerializeRenderTexture Error" << path << "\n" << std::endl;
		return;
	}

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "RenderTexture";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "width" << YAML::Value << texture->width;
	out << YAML::Key << "height" << YAML::Value << texture->height;
	out << YAML::Key << "isCube" << YAML::Value << texture->isCube;
	out << YAML::Key << "isGammaCorrection" << YAML::Value << texture->isGammaCorrection;
	out << YAML::Key << "msaa" << YAML::Value << texture->msaa;
	out << YAML::Key << "mipmapCount" << YAML::Value << texture->mipmapCount;
	out << YAML::Key << "anisotropy" << YAML::Value << texture->anisotropy;

	out << YAML::Key << "Attachments";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& attachment : texture->attachments) {
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "colorFormat" << YAML::Value << static_cast<int>(attachment.colorFormat);
		out << YAML::Key << "filter" << YAML::Value << static_cast<int>(attachment.filter);
		out << YAML::Key << "warpU" << YAML::Value << static_cast<int>(attachment.warpU);
		out << YAML::Key << "warpV" << YAML::Value << static_cast<int>(attachment.warpV);
		out << YAML::Key << "warpW" << YAML::Value << static_cast<int>(attachment.warpW);
		out << YAML::Key << "isRenderBuffer" << YAML::Value << attachment.isRenderBuffer;
		out << YAML::EndMap;
	}
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
		std::cout << "YamlSerializer::SerializeRenderTexture Error" << path << "\n" << std::endl;
	}

}

Ref<RenderTexture> YamlSerializer::DeSerializeRenderTexture(const char* path)
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

	data = data["RenderTexture"];
	if (!data)
		return nullptr;

	int width = data["width"].as<int>();
	int height = data["height"].as<int>();
	int msaa = data["msaa"].as<int>();
	int mipmapCount = data["mipmapCount"].as<int>();
	int anisotropy = data["anisotropy"].as<int>();
	bool isCube = data["isCube"].as<bool>();
	bool isGammaCorrection = data["isGammaCorrection"].as<bool>();

	std::vector<FramebufferTextureSpecification> array;
	auto attachments = data["Attachments"];
	for (auto attachment : attachments)
	{
		FramebufferTextureSpecification fts;
		fts.colorFormat = static_cast<TextureFormat>(attachment["colorFormat"].as<int>());
		fts.filter = static_cast<FilterMode>(attachment["filter"].as<int>());
		fts.warpU = static_cast<TextureWrapMode>(attachment["warpU"].as<int>());
		fts.warpV = static_cast<TextureWrapMode>(attachment["warpV"].as<int>());
		fts.warpW = static_cast<TextureWrapMode>(attachment["warpW"].as<int>());
		fts.isRenderBuffer = attachment["isRenderBuffer"].as<bool>();
		array.push_back(fts);
	}

	return RenderTextureManager::Get(width, height, array, msaa, mipmapCount, isCube, isGammaCorrection, anisotropy);
}
