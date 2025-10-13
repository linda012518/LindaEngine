#include "ShaderLoader.h"
#include "TextLoader.h"
#include "ShaderManager.h"
#include "ShaderAttribute.h"

#include <sstream>
#include <iostream>
#include <regex>

using namespace LindaEngine;

std::vector<ShaderSource> ShaderLoader::Load(const char* url)
{
	std::string tex = TextLoader::Load(url);

	std::vector<std::string> paths;
	GetFilePaths(url, paths);

	DeleteShaderFrame(tex);

	ProcessInclude(tex, paths);

	return GetPasses(url, tex);
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

			tex.replace(firstPos, two - firstPos + 1, ShaderManager::GetInclude(go.c_str()) + "\n");
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

std::string ShaderLoader::GetPassName(std::string& tex)
{
	std::string includes = "Name";

	size_t firstPos = tex.find(includes);
	if (firstPos == std::string::npos)
	{
		std::string go = "Color";
		return go;
	}

	size_t one = tex.find('"', firstPos + 1);
	size_t two = tex.find('"', one + 1);

	std::string name = tex.substr(one + 1, two - one - 1);
	tex = tex.substr(two + 1);

	return name;
}

std::vector<ShaderSource> ShaderLoader::GetPasses(std::string url, std::string& tex)
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

	std::vector<ShaderSource> shaders;

	for (std::string pass : passes)
	{
		ShaderSource ss;

		ss.name = GetPassName(pass);

		size_t vertexPos = pass.find("Vertex");
		size_t fragPos = pass.find("Fragment");

		firstPos = pass.find('{', vertexPos);
		lastPos = pass.rfind('}', fragPos);

		std::string vertex = pass.substr(firstPos + 1, lastPos - firstPos - 1);

		firstPos = pass.find('{', fragPos);
		lastPos = pass.rfind('}');

		std::string fragment = pass.substr(firstPos + 1, lastPos - firstPos - 1);

		ss.vertex = vertex;
		ss.fragment = fragment;

		CollectAttributes(ss);
		CollectUniforms(vertex);
		CollectUniforms(fragment);

		shaders.push_back(ss);
	}

	return shaders;
}

void ShaderLoader::CollectAttributes(ShaderSource& ss)
{
	//替换AttributeNames字符串为layout
	//再收集AttributeNames留给Mesh生成VAO
}

void ShaderLoader::CollectUniforms(std::string& tex)
{
	//删除Uniforms包装
	//提取uniform名字和类型留给Material自动传数据
}

void ShaderLoader::AddGlobalContent(ShaderSource& ss)
{
	//加入UniformBlock
	//加入LOD LogDepth等
}
