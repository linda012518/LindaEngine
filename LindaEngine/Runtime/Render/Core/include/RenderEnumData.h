#pragma once

namespace LindaEngine
{
	enum class RenderType
	{
		Opaque, Skybox, Transparent, Overlay
	};

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
		None, R8, R16, R32, RG8, RG16, RG32, RGB8, RGB16, RGB32, RGBA8, RGBA16, RGBA32, Depth16, Depth24, Depth32, SRGB8, SRGBA8
	};

	enum class TextureWrapMode
	{
		Repeat, Clamp, Mirror
	};

	enum class FilterMode
	{
		Point, Bilinear, Trilinear
	};

}
