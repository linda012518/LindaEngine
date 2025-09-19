#pragma once

#include "LObject.hpp"
#include "MaterialPass.h"
#include <vector>

namespace LindaEngine
{
	class Material : public LObject
	{
	public:
		Material();
		virtual ~Material();

	private:
		Ref<MaterialPass> _depthPass = nullptr;
		Ref<MaterialPass> _shadowCasterPass = nullptr;
		Ref<MaterialPass> _depthNormalPass = nullptr;

		std::vector<MaterialPass> _passes;
	};
}
