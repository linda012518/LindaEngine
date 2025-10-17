#pragma once

#include "AutoPtr.h"

namespace LindaEngine
{
	struct Texture2D;

	class TextureLoader
	{
	public:
		static Ref<Texture2D> Load(const char* path, bool gammaCorrection = false);
	};
}
