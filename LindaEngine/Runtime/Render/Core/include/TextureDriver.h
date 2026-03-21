#pragma once

#include "AutoPtr.h"
#include "RenderEnumData.h"
#include "glm/glm.hpp"

#include <vector>

namespace LindaEngine
{
	struct Texture;
	struct Cubemap;
	class TextureContext;
	struct RenderTexture;
	class Material;

	class TextureDriver
	{
	public:
		static void CreateTexture2D(Ref<Texture> texture, void* data, int channels, int bitCount);
		static void CreateCube(Ref<Texture> texture, void* right, void* left, void* top, void* bottom, void* front, void* back, int channels, int bitCount);
		static void CreateCubeByPanoramic(Ref<Texture> src, Ref<Texture> dest);
		static void CreateCubeByMaterial(Ref<Material> material, Ref<Texture> dest);
		static void CreateIBLIrradianceMap(Ref<Texture> src, Ref<Texture> dest);
		static void CreateIBLPrefilterMap(Ref<Texture> src, Ref<Texture> dest);
		static void CreateIBLBRDFMap(Ref<Texture> dest);
		static void DeleteTexture(Ref<Texture> texture);
		static void Bind(Ref<Texture> texture, int channel, int renderTextureColorIndex = 0);
		static void BindRenderTarget(Ref<RenderTexture> texture);
		static void CreateRenderTexture(Ref<RenderTexture> rt);
		static void DeleteRenderTexture(Ref<RenderTexture> rt);
		static void CopyRenderTexture(Ref<RenderTexture> src, Ref<RenderTexture> dest, ColorType type = ColorType::Color);
		static void* ReadPixed(Ref<RenderTexture> src, int xStart, int yStart, int width, int height, uint32_t attachmentIndex = 0);
		static std::vector<std::vector<glm::vec3>> GetPixelByCubemap(Ref<Texture> cubemap);

		static void Initialize();

	private:
		static Ref<TextureContext> _context;
	};
}
