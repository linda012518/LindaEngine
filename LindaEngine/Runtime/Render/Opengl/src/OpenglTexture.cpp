#include "OpenglTexture.h"
#include "Texture.h"
#include "glad/glad.h"

using namespace LindaEngine;

void OpenglTexture::CreateTexture2D(Ref<Texture> texture, void* data, int channels, int bitCount)
{
	if (nullptr == data)
		return;

	Ref<Texture2D> tex2D = DynamicCastRef(Texture2D, texture);

	GLenum dataFormat = GetDataFormat(channels);
	GLenum internalFormat = GetInternalFormat(channels, bitCount, tex2D->isGammaCorrection);
	GLenum dataType = GetDataType(bitCount);

	GetTextureFormat(tex2D, internalFormat);

	glGenTextures(1, &tex2D->nativeColorID);
	glBindTexture(GL_TEXTURE_2D, tex2D->nativeColorID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tex2D->width, tex2D->height, 0, dataFormat, dataType, data);

	if (tex2D->mipmapCount > 1)
		glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode(tex2D->warpU));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode(tex2D->warpV));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetFilterMode(tex2D->filter, tex2D->mipmapCount));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (tex2D->anisotropy > 1)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)tex2D->anisotropy);

}

void OpenglTexture::CreateCube(Ref<Texture> texture, void* right, void* left, void* top, void* bottom, void* front, void* back, int channels, int bitCount)
{
	if (nullptr == right || nullptr == left || nullptr == top || nullptr == bottom || nullptr == front || nullptr == back)
		return;

	Ref<Cubemap> cubemap = DynamicCastRef(Cubemap, texture);

	GLenum dataFormat = GetDataFormat(channels);
	GLenum internalFormat = GetInternalFormat(channels, bitCount, cubemap->isGammaCorrection);
	GLenum dataType = GetDataType(bitCount);

	GetTextureFormat(cubemap, internalFormat);

	glGenTextures(1, &cubemap->nativeColorID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->nativeColorID);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, cubemap->width, cubemap->height, 0, dataFormat, dataType, right);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, cubemap->width, cubemap->height, 0, dataFormat, dataType, left);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, cubemap->width, cubemap->height, 0, dataFormat, dataType, top);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, cubemap->width, cubemap->height, 0, dataFormat, dataType, bottom);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, cubemap->width, cubemap->height, 0, dataFormat, dataType, back);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, cubemap->width, cubemap->height, 0, dataFormat, dataType, front);

	if (cubemap->mipmapCount > 1)
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetWrapMode(cubemap->warpU));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetWrapMode(cubemap->warpV));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GetWrapMode(cubemap->warpW));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GetFilterMode(cubemap->filter, cubemap->mipmapCount));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (cubemap->anisotropy > 1)
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)cubemap->anisotropy);
}

void OpenglTexture::DeleteTexture(Ref<Texture> texture)
{
	if (nullptr != texture && texture->nativeColorID != 0)
		glDeleteTextures(1, &texture->nativeColorID);
}

void OpenglTexture::Bind(Ref<Texture> texture, int channel)
{
	glActiveTexture(GL_TEXTURE0 + channel);
	switch (texture->type)
	{
	case TextureType::Tex2D: glBindTexture(GL_TEXTURE_2D, texture->nativeColorID); break;
	case TextureType::Cube: glBindTexture(GL_TEXTURE_CUBE_MAP, texture->nativeColorID); break;
	}
}

int OpenglTexture::GetWrapMode(TextureWrapMode& mode)
{
	switch (mode)
	{
	case TextureWrapMode::Repeat: return GL_REPEAT;
	case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
	case TextureWrapMode::Mirror: return GL_MIRRORED_REPEAT;
	}
	return GL_REPEAT;
}

int OpenglTexture::GetFilterMode(FilterMode& mode, int mipCount)
{
	if (mipCount > 1)
	{
		switch (mode)
		{
		case FilterMode::Point: return GL_NEAREST_MIPMAP_NEAREST;
		case FilterMode::Bilinear: return GL_LINEAR_MIPMAP_LINEAR;
		case FilterMode::Trilinear: return GL_LINEAR_MIPMAP_LINEAR;
		}
	}
	else
	{
		switch (mode)
		{
		case FilterMode::Point: return GL_NEAREST;
		case FilterMode::Bilinear: return GL_LINEAR;
		case FilterMode::Trilinear: return GL_LINEAR;
		}
	}
	return GL_LINEAR;
}

unsigned int OpenglTexture::GetDataFormat(int channels)
{
	switch (channels) {
	case 1: return GL_RED;
	case 2: return GL_RG;
	case 3: return GL_RGB;
	case 4: return GL_RGBA;
	}
	return GL_RGBA;
}

unsigned int OpenglTexture::GetInternalFormat(int channels, int bitCount, bool isGammaCorrection)
{
	unsigned int internalFormat = 0;
	if (16 == bitCount)
	{
		switch (channels) {
		case 1: internalFormat = GL_R16F; break;
		case 2: internalFormat = GL_RG16F; break;
		case 3: internalFormat = GL_RGB16F; break;
		case 4: internalFormat = GL_RGBA16F; break;
		}
	}
	else if (32 == bitCount)
	{
		switch (channels) {
		case 1: internalFormat = GL_R32F; break;
		case 2: internalFormat = GL_RG32F; break;
		case 3: internalFormat = GL_RGB32F; break;
		case 4: internalFormat = GL_RGBA32F; break;
		}
	}
	else
	{
		switch (channels) {
		case 1: internalFormat = GL_R8; break;
		case 2: internalFormat = GL_RG8; break;
		case 3: internalFormat = isGammaCorrection ? GL_SRGB8 : GL_RGB8; break;
		case 4: internalFormat = isGammaCorrection ? GL_SRGB8_ALPHA8 : GL_RGBA8; break;
		}
	}
	return internalFormat;
}

unsigned int OpenglTexture::GetDataType(int bitCount)
{
	if (16 == bitCount)
		return GL_UNSIGNED_SHORT;
	else if (32 == bitCount)
		return GL_FLOAT;
	else
		return GL_UNSIGNED_BYTE;
}

void OpenglTexture::GetTextureFormat(Ref<Texture> texture, unsigned int internalFormat)
{
	switch (internalFormat)
	{
	case GL_R16F: texture->colorFormat = TextureFormat::R16; break;
	case GL_RG16F: texture->colorFormat = TextureFormat::RG16; break;
	case GL_RGB16F: texture->colorFormat = TextureFormat::RGB16; break;
	case GL_RGBA16F: texture->colorFormat = TextureFormat::RGBA16; break;
	case GL_R32F: texture->colorFormat = TextureFormat::R32; break;
	case GL_RG32F: texture->colorFormat = TextureFormat::RG32; break;
	case GL_RGB32F: texture->colorFormat = TextureFormat::RGB32; break;
	case GL_RGBA32F: texture->colorFormat = TextureFormat::RGBA32; break;
	case GL_R8: texture->colorFormat = TextureFormat::R8; break;
	case GL_RG8: texture->colorFormat = TextureFormat::RG8; break;
	case GL_RGB8: texture->colorFormat = TextureFormat::RGB8; break;
	case GL_RGBA8: texture->colorFormat = TextureFormat::RGBA8; break;
	case GL_SRGB8: texture->colorFormat = TextureFormat::SRGB8; break;
	case GL_SRGB8_ALPHA8: texture->colorFormat = TextureFormat::SRGBA8; break;
	}

}
