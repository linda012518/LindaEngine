#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

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
		static Ref<LindaEngine::Material> GetCurrentMaterial() { return _material; }
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

	private:
		static Ref<MaterialNodeEditor> _materialNode;
		static Ref<LindaEngine::Material> _material;
		static std::vector<Ref<MaterialNodeEditor>> _materialNodes;
		static Ref<ShaderNodeEditor> _shaderNode;
		static std::vector<Ref<ShaderNodeEditor>> _shaderNodes;
	};
}

