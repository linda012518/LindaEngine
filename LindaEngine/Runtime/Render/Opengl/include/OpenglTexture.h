#pragma once

#include "AutoPtr.h"
#include "TextureContext.h"
#include "RenderEnumData.h"

namespace LindaEngine
{
	class OpenglTexture : public TextureContext
	{
	public:
		void CreateTexture2D(Ref<Texture> texture, void* data, int channels, int bitCount);
		void CreateCube(Ref<Texture> texture, void* right, void*left, void* top, void* bottom, void* front, void* back, int channels, int bitCount);
		void DeleteTexture(Ref<Texture> texture);
		void Bind(Ref<Texture> texture, int channel);
		void CreateRenderTexture(Ref<RenderTexture> rt);
		void DeleteRenderTexture(Ref<RenderTexture> rt);

	private:
		unsigned int CreateOpenglTexture2D(int width, int height, unsigned int dataFormat, unsigned int internalFormat, unsigned int dataType, FilterMode filter = FilterMode::Bilinear, TextureWrapMode warpU = TextureWrapMode::Clamp, TextureWrapMode warpV = TextureWrapMode::Clamp, int mipmapCount = 1, int anisotropy = 0, void* data = nullptr);
		unsigned int CreateOpenglCubemap(int width, int height, unsigned int dataFormat, unsigned int internalFormat, unsigned int dataType, FilterMode filter = FilterMode::Bilinear, TextureWrapMode warpU = TextureWrapMode::Clamp, TextureWrapMode warpV = TextureWrapMode::Clamp, TextureWrapMode warpW = TextureWrapMode::Clamp, int mipmapCount = 1, int anisotropy = 0, void* right = nullptr, void* left = nullptr, void* top = nullptr, void* bottom = nullptr, void* back = nullptr, void* front = nullptr);
		void CreateRenderTexture2D(Ref<RenderTexture> rt);
		void CreateRenderTextureCubemap(Ref<RenderTexture> rt);

		int GetWrapMode(TextureWrapMode& mode);
		int GetFilterMode(FilterMode& mode, int mipCount);
		unsigned int GetDataFormat(int channels);
		unsigned int GetInternalFormat(int channels, int bitCount, bool isGammaCorrection);
		unsigned int GetDataType(int bitCount);
		void GetTextureFormat(Ref<Texture> texture, unsigned int internalFormat);

		unsigned int GetRenderTextureInternalFormat(TextureFormat& format);
		unsigned int GetRenderTextureDataFormat(TextureFormat& format);
		unsigned int GetRenderTextureDataType(TextureFormat& format);
		unsigned int GetRenderTextureDepthAttachment(TextureFormat& format);

	};
}
