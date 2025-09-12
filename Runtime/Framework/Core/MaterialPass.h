#pragma once

#include "LObject.hpp"

namespace LindaEngine
{
	class Shader;

	class MaterialPass : public LObject
	{
	public:
		MaterialPass();
		virtual ~MaterialPass();

		void Use();

	private:

	};
}
