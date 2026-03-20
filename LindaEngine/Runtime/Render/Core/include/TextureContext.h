#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"
#include "glm/glm.hpp"

#include <vector>

namespace LindaEngine
{
	struct Texture;
	struct RenderTexture;

	class TextureContext
	{
	public:
		virtual void CreateTexture2D(Ref<Texture> texture, void* data, int channels, int bitCount) = 0;
		virtual void CreateCube(Ref<Texture> texture, void* right, void* left, void* top, void* bottom, void* front, void* back, int channels, int bitCount) = 0;
		virtual void CreateCubeByPanoramic(Ref<Texture> src, Ref<Texture> dest) = 0;
		virtual void CreateIBLIrradianceMap(Ref<Texture> src, Ref<Texture> dest) = 0;
		virtual void CreateIBLPrefilterMap(Ref<Texture> src, Ref<Texture> dest) = 0;
		virtual void CreateIBLBRDFMap(Ref<Texture> dest) = 0;
		virtual void DeleteTexture(Ref<Texture> texture) = 0;
		virtual void Bind(Ref<Texture> texture, int channel, int renderTextureColorIndex) = 0;
		virtual void BindRenderTarget(Ref<RenderTexture> texture) = 0;
		virtual void CreateRenderTexture(Ref<RenderTexture> rt) = 0;
		virtual void DeleteRenderTexture(Ref<RenderTexture> rt) = 0;
		virtual void CopyColor(Ref<RenderTexture> src, Ref<RenderTexture> dest, ColorType type) = 0;
		virtual void* ReadPixed(Ref<RenderTexture> src, int xStart, int yStart, int width, int height, uint32_t attachmentIndex) = 0;
		virtual std::vector<std::vector<glm::vec3>> GetPixelByCubemap(Ref<Texture> cubemap) = 0;

		static Ref<TextureContext> Create();
	};
}


