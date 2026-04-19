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
		bool CompileShader(std::string shaderPath, const std::vector<VertexAttribute>& attributes);
		bool IsCompiled() { return nullptr != _shader; }
		void Bind(Weak<Transform> transform);

		void UpdateUniforms(std::unordered_map<std::string, Ref<ShaderUniform>>& uniformNameMap);
		const std::string& GetLightMode() const { return _state.lightMode; }

		template <typename T>
		void SetUniformValue(const char* name, T val, int count = 1);

	private:
		void LoadTextures();

	private:
		MaterialPassState _state;
		std::unordered_map<std::string, Ref<ShaderUniform>> _customUniformNameMap;
		Ref<Shader> _shader = nullptr;
		static Ref<MaterialPass> overrideMatPass;
		bool _textureLoaded = false;
		int _acitveChannel = 0;

	private:
		static RenderState _currentState; //当前渲染正在用的状态
		static RenderState _defualtState;
	};
}
