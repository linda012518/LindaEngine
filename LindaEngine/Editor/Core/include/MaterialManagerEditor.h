#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"
#include "yaml-cpp/yaml.h"

#include <vector>
#include <string>

namespace LindaEditor
{
	struct MaterialNodeEditor;
	struct ShaderNodeEditor;

	class MaterialManagerEditor
	{
	public:
		static void CreateMaterial(const char* path);
		static void SaveMaterial();
		static Ref<MaterialNodeEditor> GetCurrentMaterialNode() { return _materialNode; }
		static Ref<ShaderNodeEditor> GetCurrentShaderNode() { return _shaderNode; }

		static void Initialize();

	private:
		static Ref<MaterialNodeEditor> _materialNode;
		static std::vector<Ref<MaterialNodeEditor>> _materialNodes;
		static Ref<ShaderNodeEditor> _shaderNode;
		static std::vector<Ref<ShaderNodeEditor>> _shaderNodes;
	};
}

