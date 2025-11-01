#pragma once

#include "AutoPtr.h"
#include "TextureContext.h"

namespace LindaEngine
{
	struct Texture;
	enum class TextureWrapMode;
	enum class FilterMode;

	class OpenglTexture : public TextureContext
	{
	public:
		void CreateTexture2D(Ref<Texture> texture, void* data, int channels, int bitCount);
		void CreateCube(Ref<Texture> texture, void* right, void*left, void* top, void* bottom, void* front, void* back, int channels, int bitCount);
		void DeleteTexture(Ref<Texture> texture);
		void Bind(Ref<Texture> texture, int channel);

	private:
		int GetWrapMode(TextureWrapMode& mode);
		int GetFilterMode(FilterMode& mode, int mipCount);
		unsigned int GetDataFormat(int channels);
		unsigned int GetInternalFormat(int channels, int bitCount, bool isGammaCorrection);
		unsigned int GetDataType(int bitCount);
		void GetTextureFormat(Ref<Texture> texture, unsigned int internalFormat);

	};
}
