#include "ShaderLoader.h"
#include "TextLoader.h"
#include "Core/ShaderManager.h"

#include <sstream>
#include <iostream>

using namespace LindaEngine;

std::string ShaderLoader::Load(const char* url)
{
	std::string tex = TextLoader::Load(url);

	std::vector<std::string> paths;
	GetFilePaths(url, paths);

	DeleteShaderFrame(tex);

	ProcessInclude(tex, paths);

	GetPasses(url, tex);

	return std::string();

}

void ShaderLoader::GetFilePaths(const char* path, std::vector<std::string>& paths)
{
	std::stringstream ss(path);
	std::string token;
	char delimiter = '/';

	while (getline(ss, token, delimiter)) {
		paths.push_back(token);
	}
	paths.pop_back();
}

void ShaderLoader::ProcessInclude(std::string& tex, std::vector<std::string>& paths)
{
	std::string includes = "#include";

	size_t firstPos = tex.find(includes);
	size_t lastPos = 0;

	size_t one;
	size_t two;

	while (firstPos != std::string::npos)
	{
		one = tex.find('"', firstPos + 1);
		two = tex.find('"', one + 1);
		std::string path = tex.substr(one + 1, two - one - 1);

		if (path[0] != '.')
		{
			std::string go;
			for (std::string temp : paths)
			{
				go += temp + "/";
			}
			go += path;

			tex.replace(firstPos, two - firstPos + 1, TextLoader::Load(go.c_str()) + "\n");
		}
		else
		{
			size_t count = 0;
			size_t firstSignPos = path.find("../");
			while (firstSignPos != std::string::npos)
			{
				count++;
				firstSignPos = path.find("../", firstPos + 1);
			}

			if (count > paths.size() - 1)
				break;

			path = path.substr(count * 3, path.length() - count * 3);


			count = paths.size() - count;
			std::string go;
			for (std::string temp : paths)
			{
				go += temp + "/";
				count--;
				if (count <= 0)
					break;
			}
			go += path;

			tex.replace(firstPos, two - firstPos + 1, TextLoader::Load(go.c_str()) + "\n");
		}

		firstPos = tex.find(includes);
	}

}

void ShaderLoader::DeleteShaderFrame(std::string& tex)
{
	size_t attrPos = tex.find("Shader");
	if (attrPos == std::string::npos)
		return;

	size_t firstPos = tex.find('{');
	size_t lastPos = tex.rfind('}');

	tex = tex.substr(firstPos + 1, lastPos - firstPos - 1);
}

void ShaderLoader::GetPasses(std::string url, std::string& tex)
{
	std::vector<std::string> passes;

	size_t firstPos = 0;
	size_t lastPos = 0;

	while (true)
	{
		size_t firstPassPos = tex.find("Pass");
		size_t secondPassPos = tex.find("Pass", firstPassPos + 1);

		if (secondPassPos == std::string::npos)
		{
			firstPos = tex.find('{', firstPassPos);
			lastPos = tex.rfind('}', std::string::npos);
			passes.push_back(tex.substr(firstPos + 1, lastPos - firstPos - 1));
			break;
		}
		else
		{
			firstPos = tex.find('{', firstPassPos);
			lastPos = tex.rfind('}', secondPassPos);
			passes.push_back(tex.substr(firstPos + 1, lastPos - firstPos - 1));
			tex = tex.substr(secondPassPos);
		}
	}

	std::vector<std::string> shaders;

	for (std::string pass : passes)
	{
		size_t vertexPos = pass.find("Vertex");
		size_t fragPos = pass.find("Fragment");

		firstPos = pass.find('{', vertexPos);
		lastPos = pass.rfind('}', fragPos);

		std::string vertex = pass.substr(firstPos + 1, lastPos - firstPos - 1);

		firstPos = pass.find('{', fragPos);
		lastPos = pass.rfind('}');

		std::string fragment = pass.substr(firstPos + 1, lastPos - firstPos - 1);

		ShaderManager::Instance().AddShaderSource(url, vertex, fragment);
		//std::string strVert = "#version 330 core \n";
	}

}
