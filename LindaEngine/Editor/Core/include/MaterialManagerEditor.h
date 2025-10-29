#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"
#include "yaml-cpp/yaml.h"

#include <vector>
#include <string>

namespace LindaEngine { class Material; }

namespace LindaEditor
{
	struct MaterialNodeEditor;
	struct ShaderNodeEditor;
	struct MaterialStateEditor;
	struct MaterialPassStateEditor;

	class MaterialManagerEditor
	{
	public:
		static void CreateMaterial(const char* path);
		static void SaveMaterial();
		static Ref<MaterialNodeEditor> GetCurrentMaterialNode() { return _materialNode; }
		static Ref<ShaderNodeEditor> GetCurrentShaderNode() { return _shaderNode; }

		static void Initialize();

	private:
		static LindaEngine::RenderType GetRenderType(std::string& tex);
		static int GetRenderQueue(std::string& tex);
		static bool HasFallback(std::string& tex);
		static void GetPasses(std::string& tex, MaterialStateEditor& state);
		static void GetPassName(std::string& tex, Ref<MaterialPassStateEditor> passState);
		static void CollectProperties(std::string& tex, Ref<MaterialPassStateEditor> passState);
		static void CollectKeywords(std::string& tex, Ref<MaterialPassStateEditor> passState);
		static void CollectRenderState(std::string& tex, Ref<MaterialPassStateEditor> passState);

		static std::string ExtractContent(std::string& tex, std::string mark, char front, char back, bool deleteContent = true);
		static void ParseColorMask(std::string& tex, Ref<MaterialPassStateEditor> passState);
		static void ParseDepthState(std::string& tex, Ref<MaterialPassStateEditor> passState);
		static void ParseCullFaceFunc(std::string& tex, Ref<MaterialPassStateEditor> passState);
		static void ParseStencilState(std::string& tex, Ref<MaterialPassStateEditor> passState);
		static void ParseBlendState(std::string& tex, Ref<MaterialPassStateEditor> passState);
		static void ParsePolygonModeState(std::string& tex, Ref<MaterialPassStateEditor> passState);

		static void SerializeMaterialPass(YAML::Emitter& out, Ref<MaterialPassStateEditor> pass);
		static void SerializeMaterialUniform(YAML::Emitter& out, void* uniform);

	private:
		static Ref<MaterialNodeEditor> _materialNode;
		static std::vector<Ref<MaterialNodeEditor>> _materialNodes;
		static Ref<ShaderNodeEditor> _shaderNode;
		static std::vector<Ref<ShaderNodeEditor>> _shaderNodes;
	};
}

