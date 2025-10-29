#include "MaterialManagerEditor.h"
#include "TextLoader.h"
#include "NodeEditor.h"
#include "MaterialStateEditor.h"
#include "Path.h"
#include "ShaderLoader.h"
#include "ShaderUniform.h"
#include "RenderState.h"

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
	MaterialStateEditor state;
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

		state.hasFallback = HasFallback(tex);
		state.renderQueue = GetRenderQueue(tex);
		state.renderType = static_cast<RenderType>(GetRenderType(tex));
		state.materialPath = Path::GetFilePath(_shaderNode->path) + Path::GetFileNameNoExtension(_shaderNode->path) + ".mat";
		state.shaderPath = _shaderNode->path;

		GetPasses(tex, state);
	}
	catch (const std::exception&)
	{
		state.isError = true;
		//TODO 写入文件
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
	std::string type = ExtractContent(tex, "RenderType", '"', '"', true);
	if (type == "") 
		return RenderType::Opaque;

	if (type == "Opaque") return RenderType::Opaque;
	else if (type == "Skybox") return RenderType::Skybox;
	else if (type == "Transparent") return RenderType::Transparent;
	else if (type == "Overlay") return RenderType::Overlay;
	else return RenderType::Opaque;
}

int MaterialManagerEditor::GetRenderQueue(std::string& tex)
{
	std::string queue = ExtractContent(tex, "RenderQueue", '"', '"', true);
	if (queue == "") 
		return 2000;

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
	std::string lightMode = ExtractContent(tex, "LightMode", '"', '"', true);
	passState->lightMode = lightMode == "" ? "Color" : lightMode;
}

void MaterialManagerEditor::CollectProperties(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string uniforms = ExtractContent(tex, "Properties", '{', '}', true);
	if (uniforms == "")
		return;

	std::regex uniform_pattern(R"(\buniform\s+(\w+)\s+(\w+)\s*(?:=\s*([^;]+))?\s*;)");
	std::smatch matches;
	std::string::const_iterator searchStart = uniforms.cbegin();

	std::string uniformCode;
	while (std::regex_search(searchStart, uniforms.cend(), matches, uniform_pattern)) {
		if (matches.size() >= 3) { // 完整匹配 + 至少2个捕获组
			std::string type = matches[1].str();
			std::string name = matches[2].str();
			//std::string value = matches[3].str();
			if (type == "int")
			{
				Ref<IntUniformData> uniform = CreateRef<IntUniformData>();
				uniform->name = name;
				uniform->value = 0;
				passState->uniformNameMap[name] = uniform;
			}
			else if (type == "ivec2" || type == "ivec3" || type == "ivec4")
			{
				Ref<Int4UniformData> uniform = CreateRef<Int4UniformData>();
				uniform->name = name;
				uniform->value = glm::ivec4();
				passState->uniformNameMap[name] = uniform;
			}
			else if (type == "float")
			{
				Ref<FloatUniformData> uniform = CreateRef<FloatUniformData>();
				uniform->name = name;
				uniform->value = 0;
				passState->uniformNameMap[name] = uniform;
			}
			else if (type == "vec2" || type == "vec3" || type == "vec4")
			{
				Ref<Float4UniformData> uniform = CreateRef<Float4UniformData>();
				uniform->name = name;
				uniform->value = glm::vec4();
				passState->uniformNameMap[name] = uniform;
			}
			else if (type == "sampler2D" || type == "samplerCube")
			{
				Ref<TextureUniformData> uniform = CreateRef<TextureUniformData>();
				uniform->name = name;
				uniform->value = "";
				passState->uniformNameMap[name] = uniform;
			}
			else
			{
				std::cout << "MaterialManagerEditor::CollectProperties Not supported <" << type << "> yet \n" << std::endl;
			}
		}
		searchStart = matches.suffix().first;
	}
}

void MaterialManagerEditor::CollectKeywords(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string keywords = ExtractContent(tex, "Keywords", '{', '}', true);
	if (keywords == "")
		return;

	passState->keywords = TextLoader::SplitString(keywords, ',');
}

void MaterialManagerEditor::CollectRenderState(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string allStates = ExtractContent(tex, "RenderState", '{', '}', true);
	if (allStates == "")
		return;
	passState->renderState = CreateRef<RenderState>();
	ParseColorMask(allStates, passState);
	ParseDepthState(allStates, passState);
	ParseCullFaceFunc(allStates, passState);
	ParseStencilState(allStates, passState);
	ParseBlendState(allStates, passState);
	ParsePolygonModeState(allStates, passState);
}

std::string MaterialManagerEditor::ExtractContent(std::string& tex, std::string mark, char front, char back, bool deleteContent)
{
	size_t markPos = tex.find(mark);
	if (markPos == std::string::npos)
		return "";

	size_t firstPos = tex.find(front, markPos + 1);
	size_t lastPos = tex.find(back, firstPos + 1);
	std::string content = tex.substr(firstPos + 1, lastPos - firstPos - 1);
	if (deleteContent)
		tex.replace(markPos, lastPos - markPos + 1, "");
	return content;
}

void MaterialManagerEditor::ParseColorMask(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string content = ExtractContent(tex, "ColorMask", '(', ')', true);
	if (content == "")
		return;
	if (content.find('A') == std::string::npos)
		passState->renderState->colorMask.A = false;
	if (content.find('R') == std::string::npos)
		passState->renderState->colorMask.R = false;
	if (content.find('G') == std::string::npos)
		passState->renderState->colorMask.G = false;
	if (content.find('B') == std::string::npos)
		passState->renderState->colorMask.B = false;
}

void MaterialManagerEditor::ParseDepthState(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string content = ExtractContent(tex, "DepthState", '(', ')', true);
	if (content == "")
		return;

	std::vector<std::string> go = TextLoader::SplitString(content, ',');
	for (auto& str : go)
	{
		std::vector<std::string> temp = TextLoader::SplitString(str, '=');
		if (temp[0] == "depthTest")
		{
			passState->renderState->depthState.depthTest = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "depthWrite")
		{
			passState->renderState->depthState.depthWrite = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "depthFunc")
		{
			passState->renderState->depthState.depthFunc = RenderStateEnumMap::depthFuncMap[temp[1]];
		}
		else
		{
			std::cout << "MaterialManagerEditor::ParseDepthState Not supported <" << temp[0] << "> yet \n" << std::endl;
		}
	}
}

void MaterialManagerEditor::ParseCullFaceFunc(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string content = ExtractContent(tex, "CullFaceFunc", '(', ')', true);
	if (content == "")
		return;
	auto& go = RenderStateEnumMap::cullFaceFuncMap;
	if (RenderStateEnumMap::cullFaceFuncMap.find(content) != RenderStateEnumMap::cullFaceFuncMap.end())
	{
		int a = 0;
	}
	passState->renderState->cullFaceFunc = RenderStateEnumMap::cullFaceFuncMap[content];
}

void MaterialManagerEditor::ParseStencilState(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string content = ExtractContent(tex, "StencilState", '(', ')', true);
	if (content == "")
		return;
	//TODO 待解析
}

void MaterialManagerEditor::ParseBlendState(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string content = ExtractContent(tex, "BlendState", '(', ')', true);
	if (content == "")
		return;

	std::vector<std::string> go = TextLoader::SplitString(content, ',');
	for (auto& str : go)
	{
		std::vector<std::string> go = TextLoader::SplitString(str, '=');
		if (go[0] == "enableBlend")
		{
			passState->renderState->blendState.enableBlend = go[1] == "true" ? true : false;
		}
		else if (go[0] == "blendSrc")
		{
			passState->renderState->blendState.blendSrc = RenderStateEnumMap::blendFuncMap[go[1]];
		}
		else if (go[0] == "blendDis")
		{
			passState->renderState->blendState.blendDis = RenderStateEnumMap::blendFuncMap[go[1]];
		}
		else
		{
			std::cout << "MaterialManagerEditor::ParseBlendState Not supported <" << go[0] << "> yet \n" << std::endl;
		}
	}

}

void MaterialManagerEditor::ParsePolygonModeState(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string content = ExtractContent(tex, "PolygonModeState", '(', ')', true);
	if (content == "")
		return;
	passState->renderState->polygonModeState.polygonMode = RenderStateEnumMap::polygonModeMap[content];
}
