#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"
#include "RenderEnumData.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	class Transform;
	class MaterialPass;
	struct VertexAttribute;

	class Material : public LObject, public ISerializable
	{
		friend class YamlSerializer;
		friend class RendererSystem;

	public:
		Material();
		virtual ~Material();

		void SetPath(const char* path) { _filePath = path; }

		void SetShader(const char* path); //动态添加的要设置shader，然后设置所有需要的属性，再调用CompileShader
		bool Bind(Transform* transform, const std::vector<VertexAttribute>& attributes);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		int _renderQueue = 2000; //渲染队列
		RenderType _renderType = RenderType::Opaque; //是否不透明物体
		std::string _filePath = "Material";
		std::string _shaderPath = "Assets/Shaders/Unlit.shader";
		bool _isError = false;
		bool _hasFallback = false; //如果有fallback，指针为空用默认的，没有fallback不渲染
		std::unordered_map<std::string, Ref<MaterialPass>> _passes;

	public:
		static Ref<Material> overrideMat;
		static std::string overrideLightMode;
	};
}
