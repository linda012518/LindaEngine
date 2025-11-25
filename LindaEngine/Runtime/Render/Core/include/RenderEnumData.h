#pragma once

namespace LindaEngine
{
	enum class RenderType
	{
		Opaque, Skybox, Transparent, Overlay, ReflectionProbe
	};

	enum class ClearColorType
	{
		Color, DepthOnly, Skybox
	};

	enum class ColorType
	{
		Color, DepthOnly, ColorDepth
	};

	enum class RenderComponentType
	{
		None, Mesh, Skybox, SkinMesh, ScreenTriangle
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
		None, R8, R16, R32, RG8, RG16, RG32, RGB8, RGB16, RGB32, RGBA8, RGBA16, RGBA32, Depth16, Depth24, Depth32, SRGB8, SRGBA8, Depth24Stencil8, Depth32Stencil8
	};

	enum class TextureWrapMode
	{
		Repeat, Clamp, Mirror
	};

	enum class FilterMode
	{
		Point, Bilinear, Trilinear
	};

	enum class DrawType
	{
		POINTS = 0x0000,
		LINES = 0x0001,
		LINE_LOOP = 0x0002,
		LINE_STRIP = 0x0003,
		TRIANGLES = 0x0004,
		TRIANGLE_STRIP = 0x0005,
		TRIANGLE_FAN = 0x0006,
		QUADS = 0x0007
	};

}
