#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "RenderEnumData.h"

#include <string>
#include <vector>

namespace LindaEngine
{
	class Texture : public LObject
	{
		friend class YamlSerializer;
	public:
		int width = -1;
		int height = -1;
        TextureFormat colorFormat = TextureFormat::None;
		unsigned int nativeColorID = 0;
		bool isLoad = false;

		TextureType type;
		bool isGammaCorrection = false;
        int mipmapCount = 1;
		int anisotropy = 1;
        FilterMode filter = FilterMode::Bilinear;
        TextureWrapMode warpU = TextureWrapMode::Clamp;
        TextureWrapMode warpV = TextureWrapMode::Clamp;
        TextureWrapMode warpW = TextureWrapMode::Clamp;
		std::string path;
		//std::string nodePath; // 如果是动态创建的，可以手动设置一个唯一路径
		bool isUserCreate = false;

		static Ref<Texture> overrideTexture;

		virtual ~Texture() = default;

		static void OnImguiRender(Texture* texture);

	protected:
		TextureType type_temp;
		bool isGammaCorrection_temp = false;
		int mipmapCount_temp = 1;
		int anisotropy_temp = 1;
		FilterMode filter_temp = FilterMode::Bilinear;
		TextureWrapMode warpU_temp = TextureWrapMode::Clamp;
		TextureWrapMode warpV_temp = TextureWrapMode::Clamp;
		TextureWrapMode warpW_temp = TextureWrapMode::Clamp;

	};

	class Texture2D : public Texture
	{
	public:
		Texture2D() { type = TextureType::Tex2D; type_temp = type; }
	};

	class Cubemap : public Texture
	{
	public:
		Cubemap() { type = TextureType::Cube; type_temp = type; }

		CubemapSrcType srcType = CubemapSrcType::EquireCtangular;

		std::string left;
		std::string right;
		std::string top;
		std::string bottom;
		std::string front;
		std::string back;
	};

	class FramebufferTextureSpecification
	{
	public:
		TextureFormat colorFormat = TextureFormat::None;
		FilterMode filter = FilterMode::Bilinear;
		TextureWrapMode warpU = TextureWrapMode::Clamp;
		TextureWrapMode warpV = TextureWrapMode::Clamp;
		TextureWrapMode warpW = TextureWrapMode::Clamp;
		bool isRenderBuffer = false;
	};

	class RenderTexture : public Texture
	{
		friend class RenderTextureManager;
	public:
		RenderTexture() { type = TextureType::RenderTexture; type_temp = type; }

		static void OnImguiRender(RenderTexture* texture);

		int msaa = 1;
		bool isCube = false;
		bool isLinkScreen = true;

		std::vector<FramebufferTextureSpecification> attachments;
		std::vector<FramebufferTextureSpecification> colorAttachments;
		FramebufferTextureSpecification depthAttachment;
		std::vector<unsigned int> nativeIDs;
		std::vector<unsigned int> renderBuffers;
		unsigned int depthNativeID = 0;
		static Ref<RenderTexture> active;
		static Ref<RenderTexture> finalRT;
	private:
		Ref<RenderTexture> internalRT;//当rt是RenderBuffer时，调用Blit需要生成对应纹理（调用者无感知）

	};
}

