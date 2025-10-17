#pragma once

#include "AutoPtr.h"

#include <unordered_map>
#include <string>

namespace LindaEngine
{
	struct Texture2D;

	class TextureManager
	{
	public:
		static Ref<Texture2D> GetTexture(const char* path);

	private:
		static std::unordered_map<std::string, Ref<Texture2D>> _textureMap;
	};

	class RenderTexturePool
	{
	public:

	private:
	};
}
