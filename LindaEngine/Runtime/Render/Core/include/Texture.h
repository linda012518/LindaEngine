#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

namespace LindaEngine
{
	struct Texture
	{
		int width = -1;
		int height = -1;
        TextureFormat colorFormat = TextureFormat::None;
		unsigned int nativeColorID = 0;
		bool isLoad = false;

		TextureType type;
		bool isDataSRGB = true;
        int mipmapCount = -1;
		int anisotropy = 0;
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
		TextureFormat stencilFormat = TextureFormat::None;
		int nativeDepthID = -1;
		int msaa = -1;
		bool isCube = false;

		static Ref<RenderTexture> active;
	};
}

