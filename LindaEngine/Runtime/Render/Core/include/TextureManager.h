#pragma once

#include "AutoPtr.h"
#include "Texture.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace LindaEngine
{
	class TextureManager
	{
	public:
		static Ref<Texture2D> GetTexture(const char* path);
		static void Clear();

	private:
		static std::unordered_map<std::string, Ref<Texture2D>> _textureMap;
	};

	class RenderTextureManager
	{
	public:
		static Ref<RenderTexture> Get(int width, int height, TextureFormat colorFormat = TextureFormat::RGB8, bool sRGB = false, int msaa = 0, int mipCount = 0, TextureFormat depthFormat = TextureFormat::None, TextureFormat stencilFormat = TextureFormat::None);
		static void Release(Ref<RenderTexture> rt);
		static void Clear();

	private:
		static std::vector<Ref<RenderTexture>> _renderTextures;
	};
}
