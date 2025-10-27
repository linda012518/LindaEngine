#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "RenderState.h"
#include "ShaderUniform.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class Transform;
	class Shader;
	struct ShaderSourceCode;
	struct VertexAttribute;

	class MaterialPass : public LObject
	{
		friend class YamlSerializer;
		friend class Material;

	public:
		void AddKeyword(std::string& key);
		void CompileShader(std::string shaderPath, const std::vector<VertexAttribute>& attributes);
		bool IsCompiled() { return nullptr != _shader; }
		void Bind(Transform* transform);

		void UpdateUniforms();
		const std::string& GetLightMode() const { return _lightMode; }

		template <typename T>
		void SetUniformValue(const char* name, T val);

	private:
		//Editor可以从shader文件解析，Runtime只从材质获取，动态创建材质用户需要知道有哪些uniform
		std::unordered_map<std::string, Ref<ShaderUniform>> _uniformNameMap;
		std::vector<std::string> _keywords; //关键字宏定义，全局的从其它配置添加，不记录
		std::string _lightMode = "Color";
		Ref<Shader> _shader = nullptr;
		RenderState _renderState; //材质需要的状态
		static Ref<MaterialPass> overrideMatPass;

		int _acitveChannel = 0;
	private:
		static RenderState _currentState; //当前渲染正在用的状态
		static RenderState _defualtState;
	};
}
