#pragma once

#include <unordered_map>
#include <string>

namespace LindaEngine
{


	class TextureManager
	{
	public:
		static unsigned int GetTexture(const char* path);

	private:
		static std::unordered_map<std::string, unsigned int> _textureMap;
	};
}
