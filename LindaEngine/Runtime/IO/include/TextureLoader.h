#pragma once

namespace LindaEngine
{
	class TextureLoader
	{
	public:
		static unsigned int Load(const char* path, bool gammaCorrection = false);
	};
}
