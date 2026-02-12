#include "TextureDriver.h"
#include "TextureContext.h"

using namespace LindaEngine;

Ref<TextureContext> TextureDriver::_context;

void TextureDriver::CreateTexture2D(Ref<Texture> texture, void* data, int channels, int bitCount)
{
	_context->CreateTexture2D(texture, data, channels, bitCount);
}

void TextureDriver::CreateCube(Ref<Texture> texture, void* right, void* left, void* top, void* bottom, void* front, void* back, int channels, int bitCount)
{
	_context->CreateCube(texture, right, left, top, bottom, front, back, channels, bitCount);
}

void TextureDriver::CreateCubeByPanoramic(Ref<Texture> src, Ref<Texture> dest)
{
	_context->CreateCubeByPanoramic(src, dest);
}

void TextureDriver::DeleteTexture(Ref<Texture> texture)
{
	_context->DeleteTexture(texture);
}

void TextureDriver::Bind(Ref<Texture> texture, int channel, int renderTextureColorIndex)
{
	_context->Bind(texture, channel, renderTextureColorIndex);
}

void TextureDriver::BindRenderTarget(Ref<RenderTexture> texture)
{
	_context->BindRenderTarget(texture);
}

void TextureDriver::CreateRenderTexture(Ref<RenderTexture> rt)
{
	_context->CreateRenderTexture(rt);
}

void TextureDriver::DeleteRenderTexture(Ref<RenderTexture> rt)
{
	_context->DeleteRenderTexture(rt);
}

void TextureDriver::CopyRenderTexture(Ref<RenderTexture> src, Ref<RenderTexture> dest, ColorType type)
{
	_context->CopyColor(src, dest, type);
}

void* TextureDriver::ReadPixed(Ref<RenderTexture> src, int xStart, int yStart, int width, int height, uint32_t attachmentIndex)
{
	return _context->ReadPixed(src, xStart, yStart, width, height, attachmentIndex);
}

void TextureDriver::Initialize()
{
	_context = TextureContext::Create();
}

