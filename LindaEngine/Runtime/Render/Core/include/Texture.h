#pragma once

#include "AutoPtr.h"

namespace LindaEngine
{
	enum class TextureType
	{
		None, Tex2D, Cube, RenderTexture
	};

	enum class CubemapSrcType
	{
		EquireCtangular, SixTexture
	};

	enum class TextureFormat
	{
		None, R8, R16, R32, RG8, RG16, RG32, RGB8, RGB16, RGB32, RGBA8, RGBA16, RGBA32, Depth16, Depth24, Depth32
	};

    enum class TextureWrapMode
    {
        Repeat, Clamp, Mirror
    };

    enum class FilterMode
    {
        Point, Bilinear, Trilinear
    };

	struct Texture
	{
		int width = -1;
		int height = -1;
        TextureFormat colorFormat = TextureFormat::None;
		int nativeColorID = -1;

		TextureType type;
		bool isDataSRGB = true;
        int mipmapCount = -1;
        FilterMode filter = FilterMode::Bilinear;
        TextureWrapMode warpU = TextureWrapMode::Clamp;
        TextureWrapMode warpV = TextureWrapMode::Clamp;
        TextureWrapMode warpW = TextureWrapMode::Clamp;
		const char* path = nullptr;

		static Ref<Texture> overrideTexture;

		virtual ~Texture() = default;
	};

	struct Texture2D : public Texture
	{
		Texture2D() { type = TextureType::Tex2D; }
	};

	struct Cubemap : public Texture
	{
		Cubemap() { type = TextureType::Cube; }

		CubemapSrcType srcType = CubemapSrcType::EquireCtangular;

		const char* left = nullptr;
		const char* right = nullptr;
		const char* top = nullptr;
		const char* bottom = nullptr;
		const char* front = nullptr;
		const char* back = nullptr;
	};

	struct RenderTexture : public Texture
	{
		RenderTexture() { type = TextureType::RenderTexture; }

		TextureFormat depthFormat = TextureFormat::None;
		int nativeDepthID = -1;
		int msaa = -1;
		bool isCube = false;

		static Ref<RenderTexture> active;
	};
}

