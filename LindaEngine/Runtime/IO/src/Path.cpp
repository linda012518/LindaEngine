#include "Path.h"

#include <sstream>
#include <iostream>
#include <regex>

using namespace LindaEngine;

const char* Path::overridePath;
const char* Path::graphicsConfig = "Assets/Config/GraphicsConfig";
const char* Path::sceneConfig = "Assets/Config/SceneConfig";

std::vector<std::string> Path::GetFileDirtcorys(const char* path)
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
