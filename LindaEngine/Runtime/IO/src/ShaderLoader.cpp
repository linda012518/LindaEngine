#include "ShaderLoader.h"
#include "TextLoader.h"
#include "ShaderManager.h"
#include "ShaderUniform.h"
#include "Path.h"
#include "RenderState.h"

#include <sstream>
#include <iostream>
#include <regex>

using namespace LindaEngine;

Ref<ShaderSource> ShaderLoader::Load(const char* url)
{
	Ref<ShaderSource> ss = CreateRef<ShaderSource>();
	try
	{
		std::string tex = TextLoader::Load(url);

		std::vector<std::string> paths = Path::GetFileDirectorys(url);

		TextLoader::DeleteAnnotation(tex);
		DeleteShaderFrame(tex);
		ProcessInclude(tex, paths);
		
		ss->state.hasFallback = HasFallback(tex);
		ss->state.renderQueue = GetRenderQueue(tex);
		ss->state.renderType = static_cast<RenderType>(GetRenderType(tex));
		ss->state.shaderPath = url;

		GetPasses(tex, ss);
	}
	catch (const std::exception&)
	{
		ss->state.isError = true;
	}
	return ss;
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

void ShaderLoader::AddPickOut(std::string& str, std::string& shaderCode)
{
	size_t pos = shaderCode.rfind('}');
	if (pos == std::string::npos)
		return;
	shaderCode.insert(pos, str);
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

RenderType ShaderLoader::GetRenderType(std::string& tex)
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

int ShaderLoader::GetRenderQueue(std::string& tex)
{
	std::string queue = ExtractContent(tex, "RenderQueue", '"', '"', true);
	if (queue == "")
		return 2000;

	return std::stoi(queue);
}

bool ShaderLoader::HasFallback(std::string& tex)
{
	size_t firstPos = tex.find("FallbackPass");
	if (firstPos == std::string::npos)
		return false;
	tex.replace(firstPos, 12, "");
	return true;
}

void ShaderLoader::GetPasses(std::string& tex, Ref<ShaderSource> ss)
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
		Ref<ShaderSourceCode> passState = CreateRef<ShaderSourceCode>();
		ss->shaderSrcCode.push_back(passState);

		GetPassName(pass, passState);
		CollectProperties(pass, passState);
		CollectKeywords(pass, passState);
		CollectRenderState(pass, passState);
		CollectShaderCode(pass, passState);
	}
}

void ShaderLoader::GetPassName(std::string& tex, Ref<ShaderSourceCode> passState)
{
	std::string lightMode = ExtractContent(tex, "LightMode", '"', '"', true);
	passState->passState.lightMode = lightMode == "" ? "Color" : lightMode;
}

void ShaderLoader::CollectProperties(std::string& tex, Ref<ShaderSourceCode> passState)
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
			passState->properties += "uniform " + type + " " + name + ";\n";

			if (type == "int")
			{
				Ref<IntUniformData> uniform = CreateRef<IntUniformData>();
				uniform->name = name;
				uniform->value = std::stoi(value);
				passState->passState.uniformNameMap[name] = uniform;
			}
			else if (type == "ivec4")
			{
				size_t firstPos = value.find('[');
				size_t lastPos = value.find(']', firstPos + 1);
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
				passState->passState.uniformNameMap[name] = uniform;
			}
			else if (type == "float")
			{
				Ref<FloatUniformData> uniform = CreateRef<FloatUniformData>();
				uniform->name = name;
				uniform->value = std::stof(value);
				passState->passState.uniformNameMap[name] = uniform;
			}
			else if (type == "vec4")
			{
				size_t firstPos = value.find('[');
				size_t lastPos = value.find(']', firstPos + 1);
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
				passState->passState.uniformNameMap[name] = uniform;
			}
			else if (type == "sampler2D" || type == "samplerCube")
			{
				Ref<TextureUniformData> uniform = CreateRef<TextureUniformData>();
				uniform->name = name;
				uniform->value = value;
				passState->passState.uniformNameMap[name] = uniform;
			}
			else
			{
				std::cout << "ShaderLoader::CollectProperties Not supported <" << type << "> yet \n" << std::endl;
			}
		}
		searchStart = matches.suffix().first;
	}
}

void ShaderLoader::CollectKeywords(std::string& tex, Ref<ShaderSourceCode> passState)
{
	std::string keywords = ExtractContent(tex, "Keywords", '{', '}', true);
	if (keywords == "")
		return;

	passState->passState.keywords = TextLoader::SplitString(keywords, ',');
}

void ShaderLoader::CollectRenderState(std::string& tex, Ref<ShaderSourceCode> passState)
{
	std::string allStates = ExtractContent(tex, "RenderState", '{', '}', true);
	if (allStates == "")
		return;
	passState->passState.renderState = CreateRef<RenderState>();
	ParseColorMask(allStates, passState);
	ParseDepthState(allStates, passState);
	ParseCullFaceFunc(allStates, passState);
	ParseStencilState(allStates, passState);
	ParseBlendState(allStates, passState);
	ParsePolygonModeState(allStates, passState);
}

void ShaderLoader::CollectShaderCode(std::string& tex, Ref<ShaderSourceCode> passState)
{
	size_t vertexPos = tex.find("Vertex");
	size_t fragPos = tex.find("Fragment");

	size_t firstPos = tex.find('{', vertexPos);
	size_t lastPos = tex.rfind('}', fragPos);

	std::string vertex = tex.substr(firstPos + 1, lastPos - firstPos - 1);

	firstPos = tex.find('{', fragPos);
	lastPos = tex.rfind('}');

	std::string fragment = tex.substr(firstPos + 1, lastPos - firstPos - 1);

	std::regex pattern(R"(layout\s*\(\s*location\s*=\s*(\d+)\s*\)\s*out\s+(\w+)\s+(\w+)\s*;)");
	std::smatch matches;
	std::string::const_iterator searchStart = fragment.cbegin();
	int outColorCount = 0;
	while (std::regex_search(searchStart, fragment.cend(), matches, pattern)) {
		outColorCount++;
		searchStart = matches.suffix().first;
	}
	passState->outColorCount = outColorCount == 0 ? 1 : outColorCount;

	size_t attrPos = vertex.find("AttributeNameArray");
	if (attrPos != std::string::npos)
	{
		firstPos = vertex.find('{', attrPos);
		lastPos = vertex.find('}', firstPos + 1);
		vertex.replace(attrPos, lastPos - attrPos + 1, "");
	}

	passState->vertex = passState->properties + vertex;
	passState->fragment = passState->properties + fragment;
}

void ShaderLoader::ParseColorMask(std::string& tex, Ref<ShaderSourceCode> passState)
{
	std::string content = ExtractContent(tex, "ColorMask", '(', ')', true);
	if (content == "")
		return;
	if (content.find('A') == std::string::npos)
		passState->passState.renderState->colorMask.A = false;
	if (content.find('R') == std::string::npos)
		passState->passState.renderState->colorMask.R = false;
	if (content.find('G') == std::string::npos)
		passState->passState.renderState->colorMask.G = false;
	if (content.find('B') == std::string::npos)
		passState->passState.renderState->colorMask.B = false;
}

void ShaderLoader::ParseDepthState(std::string& tex, Ref<ShaderSourceCode> passState)
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
			passState->passState.renderState->depthState.depthTest = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "depthWrite")
		{
			passState->passState.renderState->depthState.depthWrite = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "depthFunc")
		{
			passState->passState.renderState->depthState.depthFunc = RenderStateEnumMap::depthFuncMap[temp[1]];
		}
		else
		{
			std::cout << "MaterialManagerEditor::ParseDepthState Not supported <" << temp[0] << "> yet \n" << std::endl;
		}
	}
}

void ShaderLoader::ParseCullFaceFunc(std::string& tex, Ref<ShaderSourceCode> passState)
{
	std::string content = ExtractContent(tex, "CullFaceFunc", '(', ')', true);
	if (content == "")
		return;
	passState->passState.renderState->cullFaceFunc = RenderStateEnumMap::cullFaceFuncMap[content];
}

void ShaderLoader::ParseStencilState(std::string& tex, Ref<ShaderSourceCode> passState)
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
			passState->passState.renderState->stencilState.stencilTest = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "stencilWriteMask")
		{
			passState->passState.renderState->stencilState.stencilWriteMask = std::stoi(temp[1]);
		}
		else if (temp[0] == "stencilFail")
		{
			passState->passState.renderState->stencilState.stencilFail = RenderStateEnumMap::stencilOpMap[temp[1]];
		}
		else if (temp[0] == "stencilZFail")
		{
			passState->passState.renderState->stencilState.stencilZFail = RenderStateEnumMap::stencilOpMap[temp[1]];
		}
		else if (temp[0] == "stencilZPass")
		{
			passState->passState.renderState->stencilState.stencilZPass = RenderStateEnumMap::stencilOpMap[temp[1]];
		}
		else if (temp[0] == "stencilFunc")
		{
			passState->passState.renderState->stencilState.stencilFunc = RenderStateEnumMap::stencilFuncMap[temp[1]];
		}
		else if (temp[0] == "stencilRef")
		{
			passState->passState.renderState->stencilState.stencilRef = std::stoi(temp[1]);
		}
		else if (temp[0] == "stencilFuncMask")
		{
			passState->passState.renderState->stencilState.stencilFuncMask = std::stoi(temp[1]);
		}
		else
		{
			std::cout << "MaterialManagerEditor::ParseStencilState Not supported <" << temp[0] << "> yet \n" << std::endl;
		}
	}

}

void ShaderLoader::ParseBlendState(std::string& tex, Ref<ShaderSourceCode> passState)
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
			passState->passState.renderState->blendState.enableBlend = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "blendSrc")
		{
			passState->passState.renderState->blendState.blendSrc = RenderStateEnumMap::blendFuncMap[temp[1]];
		}
		else if (temp[0] == "blendDis")
		{
			passState->passState.renderState->blendState.blendDis = RenderStateEnumMap::blendFuncMap[temp[1]];
		}
		else if (temp[0] == "enableBlendSeparate")
		{
			passState->passState.renderState->blendState.enableBlendSeparate = temp[1] == "true" ? true : false;
		}
		else if (temp[0] == "blendEqua")
		{
			passState->passState.renderState->blendState.blendEqua = RenderStateEnumMap::blendEquationMap[temp[1]];
		}
		else if (temp[0] == "blendSrcAlpha")
		{
			passState->passState.renderState->blendState.blendSrcAlpha = RenderStateEnumMap::blendFuncMap[temp[1]];
		}
		else if (temp[0] == "blendDisAlpha")
		{
			passState->passState.renderState->blendState.blendDisAlpha = RenderStateEnumMap::blendFuncMap[temp[1]];
		}
		else if (temp[0] == "constantColor")
		{
			size_t firstPos = temp[1].find('[');
			size_t lastPos = temp[1].find(']', firstPos + 1);
			temp[1] = temp[1].substr(firstPos + 1, lastPos - firstPos - 1);

			std::vector<std::string> color = TextLoader::SplitString(temp[1], ',');
			if (color.size() != 4)
				continue;
			Color c;
			c.r = std::stof(color[0]);
			c.g = std::stof(color[1]);
			c.b = std::stof(color[2]);
			c.a = std::stof(color[3]);
			passState->passState.renderState->blendState.constantColor = c;
		}
		else
		{
			std::cout << "MaterialManagerEditor::ParseBlendState Not supported <" << temp[0] << "> yet \n" << std::endl;
		}
	}

}

void ShaderLoader::ParsePolygonModeState(std::string& tex, Ref<ShaderSourceCode> passState)
{
	std::string content = ExtractContent(tex, "PolygonModeState", '(', ')', true);
	if (content == "")
		return;
	passState->passState.renderState->polygonModeState.polygonMode = RenderStateEnumMap::polygonModeMap[content];
}

std::string ShaderLoader::ExtractContent(std::string& tex, std::string mark, char front, char back, bool deleteContent)
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


