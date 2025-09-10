#pragma once

namespace LindaEngine
{
	class TextureLoader
	{
	public:
		unsigned int Load(const char* path, bool gammaCorrection = false);
	};
}
