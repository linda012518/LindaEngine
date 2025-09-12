#pragma once

#include "LObject.hpp"
#include <vector>

namespace LindaEngine
{
	class MaterialPass;

	class Material : public LObject
	{
	public:
		Material();
		virtual ~Material();

		void Use();

	private:
		std::vector<MaterialPass> _passes;
	};
}
