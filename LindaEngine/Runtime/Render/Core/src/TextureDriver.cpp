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

void TextureDriver::DeleteTexture(Ref<Texture> texture)
{
	_context->DeleteTexture(texture);
}

void TextureDriver::Bind(Ref<Texture> texture, int channel)
{
	_context->Bind(texture, channel);
}

void TextureDriver::CreateRenderTexture(Ref<RenderTexture> rt)
{
	_context->CreateRenderTexture(rt);
}

void TextureDriver::DeleteRenderTexture(Ref<RenderTexture> rt)
{
	_context->DeleteRenderTexture(rt);
}

void TextureDriver::Initialize()
{
	_context = TextureContext::Create();
}

