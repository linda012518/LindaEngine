#include "Path.h"

#include <sstream>
#include <iostream>
#include <regex>

using namespace LindaEngine;

const char* Path::overridePath;
const char* Path::graphicsConfig = "Assets/Config/GraphicsConfig";
const char* Path::sceneConfig = "Assets/Config/SceneConfig";
const char* Path::shaderConfig = "Assets/Config/ShaderConfig";

std::vector<std::string> Path::GetFileDirectorys(const char* path)
{
	std::vector<std::string> go;

	std::stringstream ss(path);
	std::string token;
	char delimiter = '/';

	while (getline(ss, token, delimiter)) {
		go.push_back(token);
	}
	go.pop_back();

	return go;
}

std::string Path::GetFilePath(std::string& path)
{
	size_t pos = path.rfind('/');
	if (pos == std::string::npos)
		return "";

	return path.substr(0, pos + 1);
}

std::string Path::GetFileName(std::string& path)
{
	size_t pos = path.rfind('/');
	if (pos == std::string::npos)
		return "";

	return path.substr(pos + 1);
}

std::string Path::GetFileNameNoExtension(std::string& path)
{
	std::string name = GetFileName(path);
	size_t pos = name.rfind('.');
	if (pos == std::string::npos)
		return name;
	return name.substr(0, pos);
}

std::string Path::GetFileExtensionName(std::string& path)
{
	std::string name = GetFileName(path);
	size_t pos = name.rfind('.');
	if (pos == std::string::npos)
		return "";
	return name.substr(pos + 1);
}
