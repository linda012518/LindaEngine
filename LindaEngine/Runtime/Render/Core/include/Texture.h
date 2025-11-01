#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

#include <string>

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
		bool isGammaCorrection = false;
        int mipmapCount = -1;
		int anisotropy = 0;
        FilterMode filter = FilterMode::Bilinear;
        TextureWrapMode warpU = TextureWrapMode::Clamp;
        TextureWrapMode warpV = TextureWrapMode::Clamp;
        TextureWrapMode warpW = TextureWrapMode::Clamp;
		std::string path;

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

		std::string left;
		std::string right;
		std::string top;
		std::string bottom;
		std::string front;
		std::string back;
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

