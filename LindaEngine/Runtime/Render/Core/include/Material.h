#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"

#include <vector>
#include <string>

namespace LindaEngine
{
	class MaterialPass;

	enum class RenderType
	{
		Opaque, Skybox, Transparent, Overlay
	};

	class Material : public LObject, public ISerializable
	{
		friend class YamlSerializer;

	public:
		Material();
		virtual ~Material();

		MaterialPass* GetDepthPass() { return _depthPass.get(); }
		MaterialPass* GetDepthNormalPass() { return _depthNormalPass.get(); }
		MaterialPass* GetShadowCasterPass() { return _shadowCasterPass.get(); }
		std::vector<Ref<MaterialPass>>& GetColorPass() { return _colorPasses; }

		void SetPath(const char* path) { _filePath = path; }

		void CompileShader(); //序列化出来的直接编译
		void SetShader(const char* path); //动态添加的要设置shader，然后设置所有需要的属性，再调用CompileShader

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		//这几个pass哪个是nullptr并且_hasFallback=true，就用defaultPass
		Ref<MaterialPass> _depthPass = nullptr;
		Ref<MaterialPass> _shadowCasterPass = nullptr;
		Ref<MaterialPass> _depthNormalPass = nullptr;

		std::vector<Ref<MaterialPass>> _colorPasses;

		int _renderQueue = 2000; //渲染队列
		RenderType _renderType = RenderType::Opaque; //是否不透明物体
		bool _shadowCast = true; //是否投射阴影
		bool _receiveShadow = true; //是否接收阴影
		std::string _filePath = "Material";
		std::string _shaderPath = "Assets/Shaders/Unlit.shader";

		bool _hasFallback = false; //如果有fallback，指针为空用默认的，没有fallback不渲染

	public:
		static Ref<Material> overrideMat;
		static Ref<MaterialPass> defaultDepthPass;
		static Ref<MaterialPass> defaultDepthNormalPass;
		static Ref<MaterialPass> defaultShadowCasterPass;
	};
}
