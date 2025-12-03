#include "OpenglTexture.h"
#include "Texture.h"
#include "glad/glad.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Entity.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "GraphicsContext.h"

#include <iostream>

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

	tex2D->nativeColorID = CreateOpenglTexture2D(tex2D->width, tex2D->height, dataFormat, internalFormat, dataType, tex2D->filter,
		tex2D->warpU, tex2D->warpV, tex2D->mipmapCount, tex2D->anisotropy, data);
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

	cubemap->nativeColorID = CreateOpenglCubemap(cubemap->width, cubemap->height, dataFormat, internalFormat, dataType, cubemap->filter,
		cubemap->warpU, cubemap->warpV, cubemap->warpW, cubemap->mipmapCount, cubemap->anisotropy,
		right, left, top, bottom, back, front);
}

void OpenglTexture::CreateCubeByPanoramic(Ref<Texture> src, Ref<Texture> dest)
{
	//TODO srcÊÇÈ«¾°Í¼ destÊÇcubemap
	FramebufferTextureSpecification color;
	color.colorFormat = TextureFormat::RGBA8;

	FramebufferTextureSpecification depth;
	depth.colorFormat = TextureFormat::Depth16;
	depth.isRenderBuffer = true;

	Ref<RenderTexture> rt = CreateRef<RenderTexture>();
	rt->width = 512;
	rt->height = 512;
	rt->isCube = true;
	rt->isGammaCorrection = false;
	rt->msaa = 1;
	rt->mipmapCount = 6;
	rt->anisotropy = 1;
	rt->colorAttachments.push_back(color);
	rt->depthAttachment = depth;
	CreateRenderTextureCubemap(rt);
	dest->nativeColorID = rt->nativeIDs[0];

	glBindFramebuffer(GL_FRAMEBUFFER, rt->nativeColorID);
	glViewport(0, 0, 512, 512);

	Entity entity("temp");
	CubeCamera* camera = entity.AddComponent<CubeCamera>();
	camera->Tick();
	Ref<Material> material = MaterialManager::GetMaterialByShader("Assets/Shaders/PanoramicToCubemap.shader");
	material->SetTexture("skybox", src);

	std::string temp = Material::overrideLightMode;
	Material::overrideLightMode = "Color";

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, src->nativeColorID);
	for (unsigned int i = 0; i < 6; ++i)
	{
		material->SetMat4("viewProjection", camera->GetVPMatrix(i));
		material->Bind(0, nullptr, MeshManager::GetSkybox()->GetMeshAttributes());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dest->nativeColorID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		MeshManager::GetSkybox()->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &rt->nativeColorID);
	glDeleteRenderbuffers((int)rt->renderBuffers.size(), rt->renderBuffers.data());

	Material::overrideLightMode = temp;
}

void OpenglTexture::DeleteTexture(Ref<Texture> texture)
{
	if (nullptr != texture && texture->nativeColorID != 0)
		glDeleteTextures(1, &texture->nativeColorID);
}

void OpenglTexture::Bind(Ref<Texture> texture, int channel, int renderTextureColorIndex)
{
	if (nullptr == texture)
		return;

	glActiveTexture(GL_TEXTURE0 + channel);
	switch (texture->type)
	{
	case TextureType::Tex2D: glBindTexture(GL_TEXTURE_2D, texture->nativeColorID); break;
	case TextureType::Cube: glBindTexture(GL_TEXTURE_CUBE_MAP, texture->nativeColorID); break;
	case TextureType::RenderTexture:
	{
		Ref<RenderTexture> rt = DynamicCastRef(RenderTexture, texture);
		if (-1 != renderTextureColorIndex)
		{
			if (false == rt->isCube)
				glBindTexture(GL_TEXTURE_2D, rt->nativeIDs[renderTextureColorIndex]);
			else
				glBindTexture(GL_TEXTURE_CUBE_MAP, rt->nativeIDs[renderTextureColorIndex]);
		}
		else
		{
			if (false == rt->isCube)
				glBindTexture(GL_TEXTURE_2D, rt->depthNativeID);
			else
				glBindTexture(GL_TEXTURE_CUBE_MAP, rt->depthNativeID);
		}
	}
	break;
	}
}

void OpenglTexture::BindRenderTarget(Ref<RenderTexture> texture)
{
	int cameraMSAA = nullptr == Camera::currentRenderCamera ? GraphicsContext::graphicsConfig.msaaSamples : Camera::currentRenderCamera->GetMSAA();
	int msaa = nullptr == texture ? cameraMSAA : texture->msaa;

	if (msaa > 1)
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);

	if (nullptr == texture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, texture->nativeColorID);
}

void OpenglTexture::CreateRenderTexture(Ref<RenderTexture> rt)
{
	if (false == rt->isCube)
		CreateRenderTexture2D(rt);
	else
		CreateRenderTextureCubemap(rt);
}

void OpenglTexture::DeleteRenderTexture(Ref<RenderTexture> rt)
{
	glDeleteFramebuffers(1, &rt->nativeColorID);
	glDeleteTextures((int)rt->nativeIDs.size(), rt->nativeIDs.data());
	glDeleteTextures(1, &rt->depthNativeID);
	glDeleteRenderbuffers((int)rt->renderBuffers.size(), rt->renderBuffers.data());
}

void OpenglTexture::CopyColor(Ref<RenderTexture> src, Ref<RenderTexture> dest, ColorType type)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src->nativeColorID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest->nativeColorID);

	unsigned int copyType = 0;
	switch (type)
	{
	case ColorType::Color: copyType = GL_COLOR_BUFFER_BIT; break;
	case ColorType::DepthOnly: copyType = GL_DEPTH_BUFFER_BIT; break;
	case ColorType::ColorDepth: copyType = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT; break;
	}

	glBlitFramebuffer(0, 0, src->width, src->height, 0, 0, dest->width, dest->height, copyType, GL_LINEAR);
}

unsigned int OpenglTexture::CreateOpenglTexture2D(int width, int height, unsigned int dataFormat, unsigned int internalFormat, unsigned int dataType, FilterMode filter, TextureWrapMode warpU, TextureWrapMode warpV, int mipmapCount, int anisotropy, void* data)
{
	unsigned int nativeID;
	glGenTextures(1, &nativeID);
	glBindTexture(GL_TEXTURE_2D, nativeID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, data);

	if (mipmapCount > 1)
		glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapMode(warpU));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapMode(warpV));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetFilterMode(filter, mipmapCount));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetFilterMode(filter, mipmapCount));

	if (anisotropy > 1)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)anisotropy);
	return nativeID;
}

unsigned int OpenglTexture::CreateOpenglCubemap(int width, int height, unsigned int dataFormat, unsigned int internalFormat, unsigned int dataType, FilterMode filter, TextureWrapMode warpU, TextureWrapMode warpV, TextureWrapMode warpW, int mipmapCount, int anisotropy, void* right, void* left, void* top, void* bottom, void* back, void* front)
{
	unsigned int nativeID;
	glGenTextures(1, &nativeID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, nativeID);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, width, height, 0, dataFormat, dataType, right);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, width, height, 0, dataFormat, dataType, left);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, width, height, 0, dataFormat, dataType, top);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, width, height, 0, dataFormat, dataType, bottom);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, width, height, 0, dataFormat, dataType, front);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, width, height, 0, dataFormat, dataType, back);

	if (mipmapCount > 1)
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetWrapMode(warpU));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetWrapMode(warpV));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GetWrapMode(warpW));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GetFilterMode(filter, mipmapCount));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GetFilterMode(filter, mipmapCount));

	if (anisotropy > 1)
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)anisotropy);

	return nativeID;
}

void OpenglTexture::CreateRenderTexture2D(Ref<RenderTexture> rt)
{
	glGenFramebuffers(1, &rt->nativeColorID);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->nativeColorID);

	int index = 0;
	std::vector<GLenum> buffers;
	for (auto& tex : rt->colorAttachments)
	{
		GLenum internalFormat = GetRenderTextureInternalFormat(tex.colorFormat);
		GLenum format = GetRenderTextureDataFormat(tex.colorFormat);
		GLenum dataType = GetRenderTextureDataType(tex.colorFormat);

		if (rt->msaa > 1)
		{
			CreateRenderBuffer(rt->msaa, internalFormat, rt->width, rt->height, GL_COLOR_ATTACHMENT0 + index);
		}
		else
		{
			if (false == tex.isRenderBuffer)
			{
				unsigned int textureColorbuffer = CreateOpenglTexture2D(rt->width, rt->height, format, internalFormat, dataType, tex.filter,
					tex.warpU, tex.warpV, rt->mipmapCount, rt->anisotropy, NULL);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, textureColorbuffer, 0);
				rt->nativeIDs.push_back(textureColorbuffer);
			}
			else
			{
				rt->renderBuffers.push_back(CreateRenderBuffer(rt->msaa, internalFormat, rt->width, rt->height, GL_COLOR_ATTACHMENT0 + index));
			}
		}
		buffers.push_back(GL_COLOR_ATTACHMENT0 + index);
		index++;
	}

	if (rt->depthAttachment.colorFormat != TextureFormat::None)
	{
		FramebufferTextureSpecification& tex = rt->depthAttachment;
		GLenum internalFormat = GetRenderTextureInternalFormat(tex.colorFormat);
		GLenum format = GetRenderTextureDataFormat(tex.colorFormat);
		GLenum dataType = GetRenderTextureDataType(tex.colorFormat);

		if (rt->msaa > 1)
		{
			CreateRenderBuffer(rt->msaa, internalFormat, rt->width, rt->height, GetRenderTextureDepthAttachment(tex.colorFormat));
		}
		else
		{
			if (rt->depthAttachment.isRenderBuffer == false)
			{
				unsigned int textureColorbuffer = CreateOpenglTexture2D(rt->width, rt->height, format, internalFormat, dataType, tex.filter,
					tex.warpU, tex.warpV, rt->mipmapCount, rt->anisotropy, NULL);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GetRenderTextureDepthAttachment(tex.colorFormat), GL_TEXTURE_2D, textureColorbuffer, 0);
				rt->depthNativeID = textureColorbuffer;
			}
			else
			{
				rt->renderBuffers.push_back(CreateRenderBuffer(rt->msaa, internalFormat, rt->width, rt->height, GetRenderTextureDepthAttachment(tex.colorFormat)));
			}
		}
	}

	if (rt->colorAttachments.size() > 1)
		glDrawBuffers((int)rt->colorAttachments.size(), &buffers[0]);
	else if (rt->colorAttachments.empty())
		glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenglTexture::CreateRenderTextureCubemap(Ref<RenderTexture> rt)
{
	glGenFramebuffers(1, &rt->nativeColorID);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->nativeColorID);

	int index = 0;
	std::vector<GLenum> buffers;
	for (auto& tex : rt->colorAttachments)
	{
		GLenum internalFormat = GetRenderTextureInternalFormat(tex.colorFormat);
		GLenum format = GetRenderTextureDataFormat(tex.colorFormat);
		GLenum dataType = GetRenderTextureDataType(tex.colorFormat);

		if (tex.isRenderBuffer == false)
		{
			unsigned int textureColorbuffer = CreateOpenglCubemap(rt->width, rt->height, format, internalFormat, dataType, rt->filter,
				rt->warpU, rt->warpV, rt->warpW, rt->mipmapCount, rt->anisotropy);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureColorbuffer, 0);
			rt->nativeIDs.push_back(textureColorbuffer);
		}
		else
		{
			rt->renderBuffers.push_back(CreateRenderBuffer(rt->msaa, internalFormat, rt->width, rt->height, GL_COLOR_ATTACHMENT0 + index));
		}

		buffers.push_back(GL_COLOR_ATTACHMENT0 + index);
		index++;
	}

	if (rt->depthAttachment.colorFormat != TextureFormat::None)
	{
		FramebufferTextureSpecification& tex = rt->depthAttachment;
		GLenum internalFormat = GetRenderTextureInternalFormat(tex.colorFormat);
		GLenum format = GetRenderTextureDataFormat(tex.colorFormat);
		GLenum dataType = GetRenderTextureDataType(tex.colorFormat);

		if (rt->depthAttachment.isRenderBuffer == false)
		{
			unsigned int textureColorbuffer = CreateOpenglCubemap(rt->width, rt->height, format, internalFormat, dataType, rt->filter,
				rt->warpU, rt->warpV, rt->warpW, rt->mipmapCount, rt->anisotropy);

			glFramebufferTexture(GL_FRAMEBUFFER, GetRenderTextureDepthAttachment(tex.colorFormat), textureColorbuffer, 0);
			rt->depthNativeID = textureColorbuffer;
		}
		else
		{
			rt->renderBuffers.push_back(CreateRenderBuffer(rt->msaa, internalFormat, rt->width, rt->height, GetRenderTextureDepthAttachment(tex.colorFormat)));
		}
	}

	if (rt->colorAttachments.size() > 1)
		glDrawBuffers((int)rt->colorAttachments.size(), &buffers[0]);
	else if (rt->colorAttachments.empty())
		glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int OpenglTexture::CreateRenderBuffer(int msaa, unsigned int internalFormat, int width, int height, unsigned int attachment)
{
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	if (msaa > 1)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, internalFormat, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo);
	return rbo;
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

unsigned int OpenglTexture::GetRenderTextureInternalFormat(TextureFormat& format)
{
	switch (format)
	{
	case TextureFormat::R8: return GL_R8;
	case TextureFormat::R16: return GL_R16F;
	case TextureFormat::R32: return GL_R32F;
	case TextureFormat::RG8: return GL_RG8;
	case TextureFormat::RG16: return GL_RG16F;
	case TextureFormat::RG32: return GL_RG32F;
	case TextureFormat::RGB8: return GL_RGB8;
	case TextureFormat::RGB16: return GL_RGB16F;
	case TextureFormat::RGB32: return GL_RGB32F;
	case TextureFormat::RGBA8: return GL_RGBA8;
	case TextureFormat::RGBA16: return GL_RGBA16F;
	case TextureFormat::RGBA32: return GL_RGBA32F;
	case TextureFormat::Depth16: return GL_DEPTH_COMPONENT16;
	case TextureFormat::Depth24: return GL_DEPTH_COMPONENT24;
	case TextureFormat::Depth32: return GL_DEPTH_COMPONENT32;
	case TextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
	case TextureFormat::Depth32Stencil8: return GL_DEPTH32F_STENCIL8;
	}
	return GL_RGBA8;
}

unsigned int OpenglTexture::GetRenderTextureDataFormat(TextureFormat& format)
{
	switch (format)
	{
	case TextureFormat::R8:
	case TextureFormat::R16:
	case TextureFormat::R32:
		return GL_RED;
	case TextureFormat::RG8:
	case TextureFormat::RG16:
	case TextureFormat::RG32:
		return GL_RG;
	case TextureFormat::RGB8:
	case TextureFormat::RGB16:
	case TextureFormat::RGB32:
		return GL_RGB;
	case TextureFormat::RGBA8:
	case TextureFormat::RGBA16:
	case TextureFormat::RGBA32:
		return GL_RGBA;
	case TextureFormat::Depth16:
	case TextureFormat::Depth24:
	case TextureFormat::Depth32:
		return GL_DEPTH_COMPONENT;
	case TextureFormat::Depth24Stencil8:
	case TextureFormat::Depth32Stencil8:
		return GL_DEPTH_STENCIL;
	}
	return GL_RGBA8;
}

unsigned int OpenglTexture::GetRenderTextureDataType(TextureFormat& format)
{
	switch (format)
	{
	case TextureFormat::R8:
	case TextureFormat::RG8:
	case TextureFormat::RGB8:
	case TextureFormat::RGBA8: 
		return GL_UNSIGNED_BYTE;
	case TextureFormat::R16:
	case TextureFormat::RG16:
	case TextureFormat::RGB16:
	case TextureFormat::RGBA16: 
	case TextureFormat::Depth16:
		return GL_UNSIGNED_SHORT;
	case TextureFormat::R32:
	case TextureFormat::RG32:
	case TextureFormat::RGB32:
	case TextureFormat::RGBA32: 
	case TextureFormat::Depth24:
	case TextureFormat::Depth32:
		return GL_FLOAT;
	case TextureFormat::Depth24Stencil8: return GL_UNSIGNED_INT_24_8;
	case TextureFormat::Depth32Stencil8: return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
	}
	return GL_UNSIGNED_BYTE;
}

unsigned int OpenglTexture::GetRenderTextureDepthAttachment(TextureFormat& format)
{
	switch (format)
	{
	case TextureFormat::Depth24Stencil8:
	case TextureFormat::Depth32Stencil8:
		return GL_DEPTH_STENCIL_ATTACHMENT;
	}
	return GL_DEPTH_ATTACHMENT;
}

