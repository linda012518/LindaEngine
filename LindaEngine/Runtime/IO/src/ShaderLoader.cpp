#include "ShaderLoader.h"
#include "TextLoader.h"
#include "ShaderManager.h"
#include "ShaderAttribute.h"

#include <sstream>
#include <iostream>
#include <regex>

using namespace LindaEngine;

ShaderSource* curSS = nullptr;

Ref<ShaderSource> ShaderLoader::Load(const char* url)
{
	std::string tex = TextLoader::Load(url);

	std::vector<std::string> paths;
	GetFilePaths(url, paths);

	DeleteShaderFrame(tex);

	ProcessInclude(tex, paths);

	Ref<ShaderSource> ss = CreateRef<ShaderSource>();
	ss->hasFallback = HasFallback(tex);
	curSS = ss.get();
	GetPasses(url, tex);
	curSS = nullptr;
	return ss;
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

bool ShaderLoader::HasFallback(std::string& tex)
{
	size_t firstPos = tex.find("FallbackPass");
	if (firstPos == std::string::npos)
		return false;
	tex.replace(firstPos, 12, "");
	return true;
}

std::string ShaderLoader::GetPassName(std::string& tex)
{
	std::string name = "LightMode";

	size_t firstPos = tex.find(name);
	if (firstPos == std::string::npos)
	{
		std::string go = "Color";
		return go;
	}

	size_t one = tex.find('"', firstPos + 1);
	size_t two = tex.find('"', one + 1);

	name = tex.substr(one + 1, two - one - 1);
	tex = tex.substr(two + 1);

	return name;
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

	std::vector<Ref<ShaderSourceCode>>& shaders = curSS->shaderSrcCode;

	for (std::string pass : passes)
	{
		Ref<ShaderSourceCode> ss = CreateRef<ShaderSourceCode>();

		ss->name = GetPassName(pass);

		size_t vertexPos = pass.find("Vertex");
		size_t fragPos = pass.find("Fragment");

		firstPos = pass.find('{', vertexPos);
		lastPos = pass.rfind('}', fragPos);

		std::string vertex = pass.substr(firstPos + 1, lastPos - firstPos - 1);

		firstPos = pass.find('{', fragPos);
		lastPos = pass.rfind('}');

		std::string fragment = pass.substr(firstPos + 1, lastPos - firstPos - 1);

		ss->vertex = vertex;
		ss->fragment = fragment;

		CollectAttributes(ss);
		CollectUniforms(ss, ss->vertex);
		CollectUniforms(ss, ss->fragment);

		shaders.push_back(ss);
	}
}

void ShaderLoader::CollectAttributes(Ref<ShaderSourceCode> ss)
{
	//替换AttributeNames字符串为layout
	//再收集AttributeNames留给Mesh生成VAO

	std::string& tex = ss->vertex;
	std::string attributeTitle = "AttributeNameArray";

	size_t firstPos = tex.find(attributeTitle);
	if (firstPos == std::string::npos)
	{
		return;
	}

	size_t one = tex.find('{', firstPos + 1);
	size_t two = tex.find('}', one + 1);

	std::string attributes = tex.substr(one + 1, two - one - 1);
	attributes.erase(std::remove_if(attributes.begin(), attributes.end(), isspace), attributes.end());

	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(attributes);
	while (std::getline(tokenStream, token, ',')) {
		tokens.push_back(token);
	}

	std::string layout;
	int index = 0;

	std::vector<std::string>& def = ShaderManager::defaultAttributeNames;
	for (int i = 0; i < def.size(); i++)
	{
		if (tokens.size() <= 0)
			break;
		auto itr = std::find(tokens.begin(), tokens.end(), def[i]);
		if (itr == tokens.end())
			continue;

		ss->attributeNames.push_back(def[i]);
		tokens.erase(std::remove(tokens.begin(), tokens.end(), def[i]), tokens.end());

		std::string type = GetAttrDataTypeByName(def[i], index);
		layout += "layout (location = " + std::to_string(index) + ") in " + type + def[i] + ";\n";
	}

	tex.replace(firstPos, two - firstPos + 1, layout);
}

void ShaderLoader::CollectUniforms(Ref<ShaderSourceCode> ss, std::string& tex)
{
	//删除Uniforms包装
	//提取uniform名字和类型留给Material自动传数据
	std::string uniformTitle = "UniformArray";

	size_t uniformPos = tex.find(uniformTitle);
	if (uniformPos == std::string::npos)
	{
		return;
	}

	size_t firstPos = tex.find('{', uniformPos + 1);
	size_t lastPos = tex.find('}', firstPos + 1);

	std::string uniforms = tex.substr(firstPos + 1, lastPos - firstPos - 1);

	tex.replace(uniformPos, lastPos - uniformPos + 1, uniforms);

	std::regex uniform_pattern(R"(\buniform\s+(\w+)\s+(\w+)\s*;)");
	std::smatch matches;
	std::string::const_iterator searchStart = uniforms.cbegin();

	while (std::regex_search(searchStart, uniforms.cend(), matches, uniform_pattern)) {
		if (matches.size() == 3) {
			std::string type = matches[1].str();
			ss->uniformsNameMapType[matches[2].str()] = GetUniformDataTypeByName(type);
		}
		searchStart = matches.suffix().first;
	}
}

std::string ShaderLoader::GetAttrDataTypeByName(std::string& name, int& index)
{
	if (name == "aPosition")
	{
		index = 0;
		return "vec3 ";
	}
	else if (name == "aNormal")
	{
		index++;
		return "vec3 ";
	}
	else if (name == "aTangent")
	{
		index++;
		return "vec3 ";
	}
	else if (name == "aUV0")
	{
		index++;
		return "vec2 ";
	}
	else if (name == "aUV1")
	{
		index++;
		return "vec2 ";
	}
	else if (name == "aUV2")
	{
		index++;
		return "vec2 ";
	}
	else if (name == "aUV3")
	{
		index++;
		return "vec2 ";
	}
	else if (name == "aUV4")
	{
		index++;
		return "vec2 ";
	}
	else if (name == "aUV5")
	{
		index++;
		return "vec2 ";
	}
	else if (name == "aUV6")
	{
		index++;
		return "vec2 ";
	}
	else if (name == "aUV7")
	{
		index++;
		return "vec2 ";
	}
	else if (name == "aColor")
	{
		index++;
		return "vec4 ";
	}
	else if (name == "aBoneID1")
	{
		index++;
		return "ivec4 ";
	}
	else if (name == "aBoneID2")
	{
		index++;
		return "ivec4 ";
	}
	else if (name == "aBoneWeights1")
	{
		index++;
		return "vec4 ";
	}
	else if (name == "aBoneWeights2")
	{
		index++;
		return "vec4 ";
	}
	else return " ";
}

UniformType ShaderLoader::GetUniformDataTypeByName(std::string& name)
{
	if (name == "float") return UniformType::FLOAT;
	else if (name == "vec4") return UniformType::FLOAT4;
	else if (name == "int") return UniformType::INT;
	else if (name == "ivec4") return UniformType::INT4;
	else if (name == "sampler2D" || name == "samplerCube") return UniformType::TEXTURE;
	else return UniformType::None;
}

