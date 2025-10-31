#pragma once

#include "AutoPtr.h"

namespace LindaEngine
{
	struct Texture;
	class TextureContext;

	class TextureDriver
	{
	public:
		static void CreateTexture2D(Ref<Texture> texture, void* data, int channels, int bitCount);
		static void CreateCube(Ref<Texture> texture, void* right, void* left, void* top, void* bottom, void* front, void* back, int channels, int bitCount);
		static void DeleteTexture(Ref<Texture> texture);
		static void Bind(Ref<Texture> texture, int channel);

	private:
		static void Initialize();

	private:
		static Ref<TextureContext> _context;
	};
}
