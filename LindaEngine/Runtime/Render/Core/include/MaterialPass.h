#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "RenderState.h"
#include "ShaderUniform.h"
#include "MaterialState.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class Transform;
	class Shader;
	struct VertexAttribute;

	class MaterialPass : public LObject
	{
		friend class YamlSerializer;
		friend class Material;
		friend class MaterialManager;

	public:
		void AddKeyword(std::string& key);
		void CompileShader(std::string shaderPath, const std::vector<VertexAttribute>& attributes);
		bool IsCompiled() { return nullptr != _shader; }
		void Bind(Transform* transform);

		void UpdateUniforms();
		const std::string& GetLightMode() const { return _state.lightMode; }

		template <typename T>
		void SetUniformValue(const char* name, T val, int count = 1);

	private:
		void LoadTextures();

	private:
		MaterialPassState _state;
		Ref<Shader> _shader = nullptr;
		static Ref<MaterialPass> overrideMatPass;
		bool _textureLoaded = false;

	private:
		static RenderState _currentState; //当前渲染正在用的状态
		static RenderState _defualtState;
	};
}
