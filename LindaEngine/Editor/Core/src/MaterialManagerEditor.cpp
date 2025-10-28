#include "MaterialManagerEditor.h"
#include "TextLoader.h"
#include "NodeEditor.h"
#include "MaterialStateEditor.h"
#include "Path.h"
#include "ShaderLoader.h"

#include <sstream>
#include <iostream>
#include <regex>

using namespace LindaEditor;
using namespace LindaEngine;

std::vector<Ref<MaterialNodeEditor>> MaterialManagerEditor::_materialNodes;
Ref<MaterialNodeEditor> MaterialManagerEditor::_materialNode;
Ref<ShaderNodeEditor> MaterialManagerEditor::_shaderNode = CreateRef<ShaderNodeEditor>();
std::vector<Ref<ShaderNodeEditor>> MaterialManagerEditor::_shaderNodes;

void MaterialManagerEditor::CreateMaterial(const char* path)
{
	try
	{
		if (nullptr == _shaderNode)
		{
			//TODO 加载默认材质
			return;
		}

		std::string tex = TextLoader::Load(_shaderNode->path.c_str());
		TextLoader::DeleteAnnotation(tex);
		ShaderLoader::DeleteShaderFrame(tex);

		MaterialStateEditor state;
		state.hasFallback = HasFallback(tex);
		state.renderQueue = GetRenderQueue(tex);
		state.renderType = static_cast<RenderType>(GetRenderType(tex));
		state.materialPath = Path::GetFilePath(_shaderNode->path) + Path::GetFileNameNoExtension(_shaderNode->path) + ".mat";
		state.shaderPath = _shaderNode->path;

		GetPasses(tex, state);
	}
	catch (const std::exception&)
	{

	}
}

void MaterialManagerEditor::SaveMaterial()
{

}

void MaterialManagerEditor::Initialize()
{
	//TODO 打开工程，加载所有的材质节点
}

RenderType MaterialManagerEditor::GetRenderType(std::string& tex)
{
	size_t firstPos = tex.find("RenderType");
	if (firstPos == std::string::npos)
		return RenderType::Opaque;

	size_t one = tex.find('"', firstPos + 1);
	size_t two = tex.find('"', one + 1);
	std::string type = tex.substr(one + 1, two - one - 1);
	tex.replace(firstPos, two - firstPos + 1, "");
	if (type == "Opaque") return RenderType::Opaque;
	else if (type == "Skybox") return RenderType::Skybox;
	else if (type == "Transparent") return RenderType::Transparent;
	else if (type == "Overlay") return RenderType::Overlay;
	else return RenderType::Opaque;
}

int MaterialManagerEditor::GetRenderQueue(std::string& tex)
{
	size_t firstPos = tex.find("RenderQueue");
	if (firstPos == std::string::npos)
		return 2000;

	size_t one = tex.find('"', firstPos + 1);
	size_t two = tex.find('"', one + 1);
	std::string queue = tex.substr(one + 1, two - one - 1);
	tex.replace(firstPos, two - firstPos + 1, "");
	return std::stoi(queue);
}

bool MaterialManagerEditor::HasFallback(std::string& tex)
{
	size_t firstPos = tex.find("FallbackPass");
	if (firstPos == std::string::npos)
		return false;
	tex.replace(firstPos, 12, "");
	return true;
}

void MaterialManagerEditor::GetPasses(std::string& tex, MaterialStateEditor& state)
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

	for (std::string pass : passes)
	{
		Ref<MaterialPassStateEditor> passState = CreateRef<MaterialPassStateEditor>();
		state.shaderPassState.push_back(passState);
		GetPassName(pass, passState);
		CollectProperties(pass, passState);
		CollectKeywords(pass, passState);
		CollectRenderState(pass, passState);
	}

}

void MaterialManagerEditor::GetPassName(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	size_t firstPos = tex.find("LightMode");
	if (firstPos == std::string::npos)
	{
		passState->lightMode = "Color";
		return;
	}

	size_t one = tex.find('"', firstPos + 1);
	size_t two = tex.find('"', one + 1);

	passState->lightMode = tex.substr(one + 1, two - one - 1);
	tex = tex.substr(two + 1);
}

void MaterialManagerEditor::CollectProperties(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	size_t uniformPos = tex.find("Properties");
	if (uniformPos == std::string::npos)
		return;

	size_t firstPos = tex.find('{', uniformPos + 1);
	size_t lastPos = tex.find('}', firstPos + 1);

	std::string uniforms = tex.substr(firstPos + 1, lastPos - firstPos - 1);
	tex.replace(uniformPos, lastPos - uniformPos + 1, "");

	std::regex uniform_pattern(R"(\buniform\s+(\w+)\s+(\w+)\s*(?:=\s*([^;]+))?\s*;)");
	std::smatch matches;
	std::string::const_iterator searchStart = uniforms.cbegin();

	std::string uniformCode;
	while (std::regex_search(searchStart, uniforms.cend(), matches, uniform_pattern)) {
		if (matches.size() >= 3) { // 完整匹配 + 至少2个捕获组

			std::string type = matches[1].str();
			std::string name = matches[2].str();
			std::string value = matches[3].str();





		}
		searchStart = matches.suffix().first;
	}
}

void MaterialManagerEditor::CollectKeywords(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	size_t keywordsPos = tex.find("Keywords");
	if (keywordsPos == std::string::npos)
		return;

	size_t firstPos = tex.find('{', keywordsPos + 1);
	size_t lastPos = tex.find('}', firstPos + 1);

	std::string keywords = tex.substr(firstPos + 1, lastPos - firstPos - 1);
	keywords.erase(std::remove_if(keywords.begin(), keywords.end(), isspace), keywords.end());

	std::string token;
	std::istringstream tokenStream(keywords);
	while (std::getline(tokenStream, token, ',')) {
		passState->keywords.push_back(token);
	}
	tex.replace(keywordsPos, lastPos - keywordsPos + 1, "");
}

void MaterialManagerEditor::CollectRenderState(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
}
