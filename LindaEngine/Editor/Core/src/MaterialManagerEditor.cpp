#include "MaterialManagerEditor.h"
#include "TextLoader.h"
#include "NodeEditor.h"
#include "MaterialStateEditor.h"
#include "Path.h"
#include "ShaderLoader.h"
#include "ShaderUniform.h"
#include "RenderState.h"
#include "YamlCustomType.h"

#include <sstream>
#include <iostream>
#include <regex>
#include <fstream>

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
		//TODO 写入文件 如果解析出错标记为错误材质
	}
	
	_materialNode = CreateRef<MaterialNodeEditor>();
	_materialNode->materialState = state;
	SaveMaterial();
}

void MaterialManagerEditor::SaveMaterial()
{
	if (nullptr == _materialNode)
		return;

	MaterialStateEditor& mat = _materialNode->materialState;

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Material";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "HasFallback" << YAML::Value << mat.hasFallback;
	out << YAML::Key << "IsError" << YAML::Value << mat.isError;
	out << YAML::Key << "MaterialPath" << YAML::Value << mat.materialPath;
	out << YAML::Key << "RenderQueue" << YAML::Value << mat.renderQueue;
	out << YAML::Key << "RenderType" << YAML::Value << static_cast<int>(mat.renderType);
	out << YAML::Key << "ShaderPath" << YAML::Value << mat.shaderPath;

	out << YAML::Key << "MaterialPasses";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& pass : mat.shaderPassState) {
		SerializeMaterialPass(out, pass);
	}
	out << YAML::EndSeq;

	out << YAML::EndMap;
	out << YAML::EndMap;

	try
	{
		std::ofstream fout(mat.materialPath);
		fout << out.c_str();
	}
	catch (const std::exception&)
	{
		std::cout << "MaterialManagerEditor::SaveMaterial Error" << mat.materialPath << "\n" << std::endl;
	}

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
			std::string value = matches[3].str();
			if (type == "int")
			{
				Ref<IntUniformData> uniform = CreateRef<IntUniformData>();
				uniform->name = name;
				uniform->value = std::stoi(value);
				passState->uniformNameMap[name] = uniform;
			}
			else if (type == "ivec4")
			{
				size_t firstPos = value.find('(');
				size_t lastPos = value.find(')', firstPos + 1);
				value = value.substr(firstPos + 1, lastPos - firstPos - 1);

				std::vector<std::string> temp = TextLoader::SplitString(value, ',');
				if (temp.size() != 4)
					continue;

				glm::ivec4 c;
				c.r = std::stoi(temp[0]);
				c.g = std::stoi(temp[1]);
				c.b = std::stoi(temp[2]);
				c.a = std::stoi(temp[3]);

				Ref<Int4UniformData> uniform = CreateRef<Int4UniformData>();
				uniform->name = name;
				uniform->value = c;
				passState->uniformNameMap[name] = uniform;
			}
			else if (type == "float")
			{
				Ref<FloatUniformData> uniform = CreateRef<FloatUniformData>();
				uniform->name = name;
				uniform->value = std::stof(value);
				passState->uniformNameMap[name] = uniform;
			}
			else if (type == "vec4")
			{
				size_t firstPos = value.find('(');
				size_t lastPos = value.find(')', firstPos + 1);
				value = value.substr(firstPos + 1, lastPos - firstPos - 1);

				std::vector<std::string> temp = TextLoader::SplitString(value, ',');
				if (temp.size() != 4)
					continue;

				glm::vec4 c;
				c.r = std::stof(temp[0]);
				c.g = std::stof(temp[1]);
				c.b = std::stof(temp[2]);
				c.a = std::stof(temp[3]);

				Ref<Float4UniformData> uniform = CreateRef<Float4UniformData>();
				uniform->name = name;
				uniform->value = c;
				passState->uniformNameMap[name] = uniform;
			}
			else if (type == "sampler2D" || type == "samplerCube")
			{
				Ref<TextureUniformData> uniform = CreateRef<TextureUniformData>();
				uniform->name = name;
				uniform->value = value;
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

	std::vector<std::string> go = TextLoader::SplitString(content, ';');
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
	passState->renderState->cullFaceFunc = RenderStateEnumMap::cullFaceFuncMap[content];
}

void MaterialManagerEditor::ParseStencilState(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string content = ExtractContent(tex, "StencilState", '(', ')', true);
	if (content == "")
		return;
	std::vector<std::string> go = TextLoader::SplitString(content, ';');
	for (auto& str : go)
	{
		std::vector<std::string> temp = TextLoader::SplitString(str, '=');
		if (temp[0] == "stencilTest")
		{
			passState->renderState->stencilState.stencilTest = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "stencilWriteMask")
		{
			passState->renderState->stencilState.stencilWriteMask = std::stoi(temp[1]);
		}
		else if (temp[0] == "stencilFail")
		{
			passState->renderState->stencilState.stencilFail = RenderStateEnumMap::stencilOpMap[temp[1]];
		}
		else if (temp[0] == "stencilZFail")
		{
			passState->renderState->stencilState.stencilZFail = RenderStateEnumMap::stencilOpMap[temp[1]];
		}
		else if (temp[0] == "stencilZPass")
		{
			passState->renderState->stencilState.stencilZPass = RenderStateEnumMap::stencilOpMap[temp[1]];
		}
		else if (temp[0] == "stencilFunc")
		{
			passState->renderState->stencilState.stencilFunc = RenderStateEnumMap::stencilFuncMap[temp[1]];
		}
		else if (temp[0] == "stencilRef")
		{
			passState->renderState->stencilState.stencilRef = std::stoi(temp[1]);
		}
		else if (temp[0] == "stencilFuncMask")
		{
			passState->renderState->stencilState.stencilFuncMask = std::stoi(temp[1]);
		}
		else
		{
			std::cout << "MaterialManagerEditor::ParseStencilState Not supported <" << temp[0] << "> yet \n" << std::endl;
		}
	}

}

void MaterialManagerEditor::ParseBlendState(std::string& tex, Ref<MaterialPassStateEditor> passState)
{
	std::string content = ExtractContent(tex, "BlendState", '(', ')', true);
	if (content == "")
		return;

	std::vector<std::string> go = TextLoader::SplitString(content, ';');
	for (auto& str : go)
	{
		std::vector<std::string> temp = TextLoader::SplitString(str, '=');
		if (temp[0] == "enableBlend")
		{
			passState->renderState->blendState.enableBlend = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "blendSrc")
		{
			passState->renderState->blendState.blendSrc = RenderStateEnumMap::blendFuncMap[temp[1]];
		}
		else if (temp[0] == "blendDis")
		{
			passState->renderState->blendState.blendDis = RenderStateEnumMap::blendFuncMap[temp[1]];
		}
		else if (temp[0] == "enableBlendSeparate")
		{
			passState->renderState->blendState.enableBlendSeparate = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "blendEqua")
		{
			passState->renderState->blendState.blendEqua = RenderStateEnumMap::blendEquationMap[temp[1]];
		}
		else if (temp[0] == "blendSrcAlpha")
		{
			passState->renderState->blendState.blendSrcAlpha = RenderStateEnumMap::blendFuncMap[temp[1]];
		}
		else if (temp[0] == "blendDisAlpha")
		{
			passState->renderState->blendState.blendDisAlpha = RenderStateEnumMap::blendFuncMap[temp[1]];
		}
		else if (temp[0] == "constantColor")
		{
			std::vector<std::string> color = TextLoader::SplitString(temp[1], ',');
			if (color.size() != 4)
				continue;
			Color c;
			c.r = std::stof(color[0]);
			c.g = std::stof(color[1]);
			c.b = std::stof(color[2]);
			c.a = std::stof(color[3]);
			passState->renderState->blendState.constantColor = c;
		}
		else
		{
			std::cout << "MaterialManagerEditor::ParseBlendState Not supported <" << temp[0] << "> yet \n" << std::endl;
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

void MaterialManagerEditor::SerializeMaterialPass(YAML::Emitter& out, Ref<MaterialPassStateEditor> pass)
{
	if (nullptr == pass)
		return;

	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "PassName" << YAML::Value << pass->lightMode;

	out << YAML::Key << "ShaderUniforms";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& uniform : pass->uniformNameMap) {
		SerializeMaterialUniform(out, uniform.second.get());
	}
	out << YAML::EndSeq;

	out << YAML::Key << "Keywords";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& key : pass->keywords) {
		out << key;
	}
	out << YAML::EndSeq;

	out << YAML::Key << "RenderState";
	out << YAML::Value << YAML::BeginMap;

	RenderState& curState = *pass->renderState.get();
	RenderState defaultState;

	if (curState.colorMask != defaultState.colorMask)
		out << YAML::Key << "ColorMask" << YAML::Value << curState.colorMask;

	if (curState.cullFaceFunc != defaultState.cullFaceFunc)
		out << YAML::Key << "CullFaceFunc" << YAML::Value << static_cast<int>(curState.cullFaceFunc);

	if (curState.depthState.depthTest == false)
	{
		out << YAML::Key << "DepthState";
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "DepthTest" << YAML::Value << false;
		if (curState.depthState.depthWrite == false)
			out << YAML::Key << "DepthWrite" << YAML::Value << false;
		if (curState.depthState.depthFunc != defaultState.depthState.depthFunc)
			out << YAML::Key << "DepthFunc" << YAML::Value << static_cast<int>(curState.depthState.depthFunc);
		out << YAML::EndMap;
	}

	if (curState.polygonModeState.polygonMode != defaultState.polygonModeState.polygonMode)
	{
		out << YAML::Key << "PolygonModeState";
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "PolygonMode" << YAML::Value << static_cast<int>(curState.polygonModeState.polygonMode);
		if (curState.polygonModeState.polygonMode == PolygonMode::LINE)
			out << YAML::Key << "LineWidth" << YAML::Value << curState.polygonModeState.lineWidth;
		if (curState.polygonModeState.polygonMode == PolygonMode::POINT)
			out << YAML::Key << "PointSize" << YAML::Value << curState.polygonModeState.pointSize;
		out << YAML::EndMap;
	}

	if (curState.stencilState.stencilTest == true)
	{
		out << YAML::Key << "StencilState";
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "StencilTest" << YAML::Value << true;

		if (curState.stencilState.stencilWriteMask != defaultState.stencilState.stencilWriteMask)
			out << YAML::Key << "StencilWriteMask" << YAML::Value << curState.stencilState.stencilWriteMask;

		if (curState.stencilState.stencilFuncMask != defaultState.stencilState.stencilFuncMask ||
			curState.stencilState.stencilRef != defaultState.stencilState.stencilRef ||
			curState.stencilState.stencilFunc != defaultState.stencilState.stencilFunc)
		{
			out << YAML::Key << "StencilFuncMask" << YAML::Value << curState.stencilState.stencilFuncMask;
			out << YAML::Key << "StencilRef" << YAML::Value << curState.stencilState.stencilRef;
			out << YAML::Key << "StencilFunc" << YAML::Value << static_cast<int>(curState.stencilState.stencilFunc);
		}

		if (curState.stencilState.stencilFail != defaultState.stencilState.stencilFail ||
			curState.stencilState.stencilZFail != defaultState.stencilState.stencilZFail ||
			curState.stencilState.stencilZPass != defaultState.stencilState.stencilZPass)
		{
			out << YAML::Key << "StencilFail" << YAML::Value << static_cast<int>(curState.stencilState.stencilFail);
			out << YAML::Key << "StencilZFail" << YAML::Value << static_cast<int>(curState.stencilState.stencilZFail);
			out << YAML::Key << "StencilZPass" << YAML::Value << static_cast<int>(curState.stencilState.stencilZPass);
		}
		out << YAML::EndMap;
	}

	if (curState.blendState.enableBlend == true)
	{
		out << YAML::Key << "BlendState";
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "EnableBlend" << YAML::Value << true;

		if (curState.blendState.constantColor != defaultState.blendState.constantColor)
			out << YAML::Key << "ConstantColor" << YAML::Value << curState.blendState.constantColor;

		if (curState.blendState.blendSrc != defaultState.blendState.blendSrc ||
			curState.blendState.blendDis != defaultState.blendState.blendDis)
		{
			out << YAML::Key << "BlendSrc" << YAML::Value << static_cast<int>(curState.blendState.blendSrc);
			out << YAML::Key << "BlendDis" << YAML::Value << static_cast<int>(curState.blendState.blendDis);
		}

		if (curState.blendState.enableBlendSeparate == true)
			out << YAML::Key << "EnableBlendSeparate" << YAML::Value << true;

		if (curState.blendState.blendEqua != defaultState.blendState.blendEqua)
			out << YAML::Key << "BlendEqua" << YAML::Value << static_cast<int>(curState.blendState.blendEqua);

		if (curState.blendState.blendSrcAlpha != defaultState.blendState.blendSrcAlpha ||
			curState.blendState.blendDisAlpha != defaultState.blendState.blendDisAlpha)
		{
			out << YAML::Key << "BlendSrcAlpha" << YAML::Value << static_cast<int>(curState.blendState.blendSrcAlpha);
			out << YAML::Key << "BlendDisAlpha" << YAML::Value << static_cast<int>(curState.blendState.blendDisAlpha);
		}

		out << YAML::EndMap;
	}

	out << YAML::EndMap;
	out << YAML::EndMap;

}

void MaterialManagerEditor::SerializeMaterialUniform(YAML::Emitter& out, void* uniform)
{
	ShaderUniform* su = static_cast<ShaderUniform*>(uniform);
	switch (su->dataType)
	{
	case UniformType::FLOAT:
	{
		auto pointer = static_cast<FloatUniformData*>(su);
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
		out << YAML::Key << "Value" << YAML::Value << pointer->value;
		out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
		out << YAML::EndMap;
		break;
	}
	case UniformType::FLOAT4:
	{
		auto pointer = static_cast<Float4UniformData*>(su);
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
		out << YAML::Key << "Value" << YAML::Value << pointer->value;
		out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
		out << YAML::EndMap;
		break;
	}
	case UniformType::INT:
	{
		auto pointer = static_cast<IntUniformData*>(su);
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
		out << YAML::Key << "Value" << YAML::Value << pointer->value;
		out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
		out << YAML::EndMap;
		break;
	}
	case UniformType::INT4:
	{
		auto pointer = static_cast<Int4UniformData*>(su);
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
		out << YAML::Key << "Value" << YAML::Value << pointer->value;
		out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
		out << YAML::EndMap;
		break;
	}
	case UniformType::TEXTURE:
	{
		auto pointer = static_cast<TextureUniformData*>(su);
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
		out << YAML::Key << "Value" << YAML::Value << pointer->value;
		out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
		out << YAML::EndMap;
		break;
	}
	}

}