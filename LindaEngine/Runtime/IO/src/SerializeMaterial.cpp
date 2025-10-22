#include "YamlSerializer.h"
#include "GraphicsContext.h"
#include "Material.h"
#include "MaterialPass.h"
#include "ShaderManager.h"
#include "YamlCustomType.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace LindaEngine;

void YamlSerializer::SerializeMaterial(const char* path)
{
	//注意，数组和矩阵不做序列化
	//覆盖Material::overrideMat，请提前保存
	Ref<Material> mat = Material::overrideMat;

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Material";
	out << YAML::Value << YAML::BeginMap;
	//out << YAML::Key << "FilePath" << YAML::Value << mat->_filePath;
	out << YAML::Key << "ShaderPath" << YAML::Value << mat->_shaderPath;
	out << YAML::Key << "RenderType" << YAML::Value << static_cast<int>(mat->_renderType);
	out << YAML::Key << "ShadowCast" << YAML::Value << mat->_shadowCast;
	out << YAML::Key << "ReceiveShadow" << YAML::Value << mat->_receiveShadow;
	out << YAML::Key << "HasFallback" << YAML::Value << mat->_hasFallback;
	out << YAML::Key << "RenderQueue" << YAML::Value << mat->_renderQueue;

	out << YAML::Key << "MaterialPasses";
	out << YAML::Value << YAML::BeginSeq;
	auto tempPass = MaterialPass::overrideMatPass;
	for (auto& pass : mat->_colorPasses) {
		MaterialPass::overrideMatPass = pass;
		SerializeMaterialPass(out);
	}
	MaterialPass::overrideMatPass = mat->_shadowCasterPass;
	SerializeMaterialPass(out);
	MaterialPass::overrideMatPass = mat->_depthPass;
	SerializeMaterialPass(out);
	MaterialPass::overrideMatPass = mat->_depthNormalPass;
	SerializeMaterialPass(out);
	MaterialPass::overrideMatPass = tempPass;
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
		std::cout << "YamlSerializer::SerializeMaterial Error" << path << "\n" << std::endl;
	}
}

Ref<Material> YamlSerializer::DeSerializeMaterial(const char* path)
{
	//覆盖Material::overrideMat，请提前保存
	YAML::Node data;
	try
	{
		data = YAML::LoadFile(path);
	}
	catch (const std::exception&)
	{
		return nullptr;
	}

	if (!data["Material"])
		return nullptr;

	Ref<Material> mat = CreateRef<Material>();
	auto material = data["Material"];

	//mat->_filePath = material["FilePath"].as<std::string>();
	mat->_shaderPath = material["ShaderPath"].as<std::string>();
	mat->_renderType = static_cast<RenderType>(material["RenderType"].as<int>());
	mat->_shadowCast = material["ShadowCast"].as<bool>();
	mat->_receiveShadow = material["ReceiveShadow"].as<bool>();
	mat->_hasFallback = material["HasFallback"].as<bool>();
	mat->_renderQueue = material["RenderQueue"].as<int>();

	Ref<MaterialPass> matPass = nullptr;
	auto passes = material["MaterialPasses"];
	for (auto pass : passes)
	{
		std::string passName = pass["PassName"].as<std::string>();
		matPass = CreateRef<MaterialPass>();
		matPass->_lightMode = passName;
		if (passName == "Depth")
			mat->_depthPass = matPass;
		else if (passName == "DepthNormal")
			mat->_depthNormalPass = matPass;
		else if (passName == "ShadowCaster")
			mat->_shadowCasterPass = matPass;
		else
			mat->_colorPasses.push_back(matPass);

		auto uniforms = pass["ShaderUniforms"];
		for (auto unif : uniforms)
		{
			std::string uniformName = unif["UniformName"].as<std::string>();
			UniformType dataType = static_cast<UniformType>(unif[uniformName].as<int>());
			
			switch (dataType)
			{
			case UniformType::TEXTURE:
			{
				Ref<TextureUniformData> pointer = CreateRef<TextureUniformData>();
				std::string tempStr = unif["Value"].as<std::string>();
				pointer->name = uniformName;
				pointer->value = tempStr.c_str();
				matPass->_uniformNameMap[uniformName] = pointer;
				break;
			}
			case UniformType::INT:
			{
				Ref<IntUniformData> pointer = CreateRef<IntUniformData>();
				pointer->name = uniformName;
				pointer->value = unif["Value"].as<int>();
				matPass->_uniformNameMap[uniformName] = pointer;
				break;
			}
			case UniformType::INT4:
			{
				Ref<Int4UniformData> pointer = CreateRef<Int4UniformData>();
				pointer->name = uniformName;
				pointer->value = unif["Value"].as<glm::ivec4>();
				matPass->_uniformNameMap[uniformName] = pointer;
				break;
			}
			case UniformType::FLOAT:
			{
				Ref<FloatUniformData> pointer = CreateRef<FloatUniformData>();
				pointer->name = uniformName;
				pointer->value = unif["Value"].as<float>();
				matPass->_uniformNameMap[uniformName] = pointer;
				break;
			}
			case UniformType::FLOAT4:
			{
				Ref<Float4UniformData> pointer = CreateRef<Float4UniformData>();
				pointer->name = uniformName;
				pointer->value = unif["Value"].as<glm::vec4>();
				matPass->_uniformNameMap[uniformName] = pointer;
				break;
			}
			}
		}

		auto Keywords = pass["Keywords"];
		for (std::size_t i = 0; i < Keywords.size(); i++) {
			matPass->_keywords.push_back(Keywords[i].as<std::string>());
		}

		RenderState& state = matPass->_renderState;
		auto renderState = pass["RenderState"];
		for (auto partial : renderState)
		{
			std::string partialName = partial.first.as<std::string>();
			if (partialName == "ColorMask")
				state.colorMask = renderState["ColorMask"].as<ColorMask>();
			else if (partialName == "CullFaceFunc")
				state.cullFaceFunc = static_cast<CullFaceFunc>(renderState["CullFaceFunc"].as<int>());
			else if (partialName == "DepthState")
			{
				auto depthState = renderState["DepthState"];
				for (auto ds : depthState)
				{
					std::string dsName = ds.first.as<std::string>();
					if (dsName == "DepthTest")
						state.depthState.depthTest = depthState["DepthTest"].as<bool>();
					else if (dsName == "DepthWrite")
						state.depthState.depthWrite = depthState["DepthTest"].as<bool>();
					else if (dsName == "DepthFunc")
						state.depthState.depthFunc = static_cast<DepthFunc>(depthState["DepthFunc"].as<int>());
				}
			}
			else if (partialName == "PolygonModeState")
			{
				auto polygonModeState = renderState["PolygonModeState"];
				for (auto pms : polygonModeState)
				{
					std::string pmsName = pms.first.as<std::string>();
					if (pmsName == "PolygonMode")
						state.polygonModeState.polygonMode = static_cast<PolygonMode>(polygonModeState["PolygonMode"].as<int>());
					else if (pmsName == "LineWidth")
						state.polygonModeState.lineWidth = polygonModeState["LineWidth"].as<float>();
					else if (pmsName == "PointSize")
						state.polygonModeState.pointSize = polygonModeState["PointSize"].as<float>();
				}
			}
			else if (partialName == "StencilState")
			{
				auto stencilState = renderState["StencilState"];
				for (auto ss : stencilState)
				{
					std::string ssName = ss.first.as<std::string>();
					if (ssName == "StencilTest")
						state.stencilState.stencilTest = stencilState["StencilTest"].as<bool>();
					else if (ssName == "StencilWriteMask")
						state.stencilState.stencilWriteMask = stencilState["StencilWriteMask"].as<int>();
					else if (ssName == "StencilFuncMask")
						state.stencilState.stencilFuncMask = stencilState["StencilFuncMask"].as<int>();
					else if (ssName == "StencilRef")
						state.stencilState.stencilRef = stencilState["StencilRef"].as<int>();
					else if (ssName == "StencilFunc")
						state.stencilState.stencilFunc = static_cast<StencilFunc>(stencilState["StencilFunc"].as<int>());
					else if (ssName == "StencilFail")
						state.stencilState.stencilFail = static_cast<StencilOp>(stencilState["StencilFail"].as<int>());
					else if (ssName == "StencilZFail")
						state.stencilState.stencilZFail = static_cast<StencilOp>(stencilState["StencilZFail"].as<int>());
					else if (ssName == "StencilZPass")
						state.stencilState.stencilZPass = static_cast<StencilOp>(stencilState["StencilZPass"].as<int>());
				}
			}
			else if (partialName == "BlendState")
			{
				auto blendState = renderState["BlendState"];
				for (auto bs : blendState)
				{
					std::string bsName = bs.first.as<std::string>();
					if (bsName == "EnableBlend")
						state.blendState.enableBlend = blendState["EnableBlend"].as<bool>();
					else if (bsName == "ConstantColor")
						state.blendState.constantColor = blendState["ConstantColor"].as<Color>();
					else if (bsName == "BlendSrc")
						state.blendState.blendSrc = static_cast<BlendFunc>(blendState["BlendSrc"].as<int>());
					else if (bsName == "BlendDis")
						state.blendState.blendDis = static_cast<BlendFunc>(blendState["BlendDis"].as<int>());
					else if (bsName == "EnableBlendSeparate")
						state.blendState.enableBlendSeparate = blendState["EnableBlendSeparate"].as<bool>();
					else if (bsName == "BlendEqua")
						state.blendState.blendEqua = static_cast<BlendEquation>(blendState["BlendEqua"].as<int>());
					else if (bsName == "BlendSrcAlpha")
						state.blendState.blendSrcAlpha = static_cast<BlendFunc>(blendState["BlendSrcAlpha"].as<int>());
					else if (bsName == "BlendDisAlpha")
						state.blendState.blendDisAlpha = static_cast<BlendFunc>(blendState["BlendDisAlpha"].as<int>());
				}
			}
		}
	}

	return mat;
}

void YamlSerializer::SerializeMaterialPass(YAML::Emitter& out)
{
	Ref<MaterialPass> pass = MaterialPass::overrideMatPass;
	if (nullptr == pass)
		return;
	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "PassName" << YAML::Value << pass->_lightMode;

	out << YAML::Key << "ShaderUniforms";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& uniform : pass->_uniformNameMap) {
		SerializeMaterialUniform(out, uniform.second.get());
	}
	out << YAML::EndSeq;

	out << YAML::Key << "Keywords";
	out << YAML::Value << YAML::BeginSeq;
	for (auto& key : pass->_keywords) {
		out << key;
	}
	out << YAML::EndSeq;

	out << YAML::Key << "RenderState";
	out << YAML::Value << YAML::BeginMap;

	RenderState& curState = pass->_renderState;
	RenderState& defaultState = MaterialPass::_defualtState;

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

void YamlSerializer::SerializeMaterialUniform(YAML::Emitter& out, void* uniform)
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
