#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"

#include <string>
#include <vector>

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
		std::string nodePath;
		bool isUserCreate = false;

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

	struct FramebufferTextureSpecification
	{
		TextureFormat colorFormat = TextureFormat::None;
		FilterMode filter = FilterMode::Bilinear;
		TextureWrapMode warpU = TextureWrapMode::Clamp;
		TextureWrapMode warpV = TextureWrapMode::Clamp;
		TextureWrapMode warpW = TextureWrapMode::Clamp;
		bool isRenderBuffer = false;
	};

	struct RenderTexture : public Texture
	{
		friend class RenderTextureManager;
		RenderTexture() { type = TextureType::RenderTexture; }

		int msaa = 1;
		bool isCube = false;

		std::vector<FramebufferTextureSpecification> attachments;
		std::vector<FramebufferTextureSpecification> colorAttachments;
		FramebufferTextureSpecification depthAttachment;
		std::vector<unsigned int> nativeIDs;
		std::vector<unsigned int> renderBuffers;
		unsigned int depthNativeID = 0;
		static Ref<RenderTexture> active;

	private:
		Ref<RenderTexture> internalRT;

	};
}

