#pragma once

#include "AutoPtr.h"

namespace LindaEngine
{
	struct Texture;

	class TextureLoader
	{
	public:
		static void Load(Ref<Texture> texture);
		static Ref<Texture> Load(const char* path);
		static void Delete(Ref<Texture> texture);

	private:
		struct TextureTempData
		{
			void* data = nullptr;
			int width = 0;
			int height = 0;
			int channels = 0;
			int bitCount = 0;
		};

	private:
		static TextureTempData LoadToMemory(const char* path);
		static void LoadTexture2D(Ref<Texture> texture);
		static void LoadCubemap(Ref<Texture> texture);
		

	};
}
