#pragma once

#include "AutoPtr.h"
#include "LObject.h"

#include <vector>
#include <string>

namespace LindaEngine
{
	class MaterialPass;

	enum class RenderType
	{
		Opaque, Skybox, Transparent, Overlay
	};

	class Material : public LObject
	{
		friend class YamlSerializer;

	public:
		Material();
		virtual ~Material();

		MaterialPass* GetDepthPass() { return _depthPass.get(); }
		MaterialPass* GetDepthNormalPass() { return _depthNormalPass.get(); }
		MaterialPass* GetShadowCasterPass() { return _shadowCasterPass.get(); }
		std::vector<Ref<MaterialPass>>& GetColorPass() { return _colorPasses; }

	private:
		Ref<MaterialPass> _depthPass = nullptr;
		Ref<MaterialPass> _shadowCasterPass = nullptr;
		Ref<MaterialPass> _depthNormalPass = nullptr;

		std::vector<Ref<MaterialPass>> _colorPasses;

		RenderType _renderType = RenderType::Opaque; //是否不透明物体
		bool _shadowCast = true; //是否投射阴影
		bool _receiveShadow = true; //是否接收阴影
		std::string _fileName = "Material";
		std::string _shaderPath = "Assets/Shaders/Unlit.shader";

	public:
		static Ref<Material> overrideMat;
	};
}
