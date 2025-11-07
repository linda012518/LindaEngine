#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace LindaEngine
{
	struct Texture;
	struct RenderTexture;

	class TextureManager
	{
	public:
		static Ref<Texture> GetTexture(std::string& path);
		static void Clear();
		static void Bind(Ref<Texture> texture, int channel);

	private:
		static std::unordered_map<std::string, Ref<Texture>> _textureMap;
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
