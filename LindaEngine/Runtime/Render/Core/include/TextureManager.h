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
	struct FramebufferTextureSpecification;

	class TextureManager
	{
	public:
		static Ref<Texture> GetTexture(std::string& path);
		static void DeleteTexture(Ref<Texture> texture);
		static void Clear();
		static void Bind(Ref<Texture> texture, int channel, int renderTextureColorIndex = 0);

	private:
		static std::unordered_map<std::string, Ref<Texture>> _textureMap;
	};

	class RenderTextureManager
	{
	public:
		static Ref<RenderTexture> Get(int width, int height, std::vector<FramebufferTextureSpecification>& fts, int msaa = 1, int mipCount = 1, bool isCube = false, bool isGammaCorrection = false, int anisotropy = 0, bool linkScreen = true);
		static Ref<RenderTexture> Get(int width, int height, FramebufferTextureSpecification& fts, int msaa = 1, int mipCount = 1, bool isCube = false, bool isGammaCorrection = false, int anisotropy = 0, bool linkScreen = true);
		static Ref<RenderTexture> Get(Ref<RenderTexture> src);
		static Ref<RenderTexture> GetBlitRenderTexture(Ref<RenderTexture> rt, ColorType type = ColorType::Color);
		static void BlitRenderTexture(Ref<RenderTexture> src, Ref<RenderTexture> dest, ColorType type = ColorType::Color);
		static void Release(Ref<RenderTexture> rt);
		static void DeleteImmediately(Ref<RenderTexture> rt);
		static void Clear();
		static void ClearLinkScreen();
		static void SetRenderTarget(Ref<RenderTexture> texture);

	private:
		static bool CompareAttachments(std::vector<FramebufferTextureSpecification>& left, std::vector<FramebufferTextureSpecification>& right);

	private:
		static std::vector<Ref<RenderTexture>> _renderTextures;
	};
}
