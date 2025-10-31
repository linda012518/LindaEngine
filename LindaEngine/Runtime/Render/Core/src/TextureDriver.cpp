#include "TextureDriver.h"
#include "TextureContext.h"

using namespace LindaEngine;

Ref<TextureContext> TextureDriver::_context;

void TextureDriver::CreateTexture2D(Ref<Texture> texture, void* data, int channels, int bitCount)
{
	Initialize();
	_context->CreateTexture2D(texture, data, channels, bitCount);
}

void TextureDriver::CreateCube(Ref<Texture> texture, void* right, void* left, void* top, void* bottom, void* front, void* back, int channels, int bitCount)
{
	Initialize();
	_context->CreateCube(texture, right, left, top, bottom, front, back, channels, bitCount);
}

void TextureDriver::DeleteTexture(Ref<Texture> texture)
{
	Initialize();
	_context->DeleteTexture(texture);
}

void TextureDriver::Bind(Ref<Texture> texture, int channel)
{
	Initialize();
	_context->Bind(texture, channel);
}

void TextureDriver::Initialize()
{
	if (nullptr == _context)
		_context = TextureContext::Create();
}

