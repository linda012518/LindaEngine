#pragma once

#include "AutoPtr.h"

namespace LindaEngine
{
	class Material;

	class MaterialLoader
	{
	public:
		static bool Serialize();
		static Ref<Material> Deserialize(const char* path);
	};
}
