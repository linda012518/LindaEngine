#pragma once

#include "AutoPtr.h"
#include "LObject.h"

#include <vector>

namespace LindaEngine
{
	class MaterialPass;

	class Material : public LObject
	{
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
	};
}
