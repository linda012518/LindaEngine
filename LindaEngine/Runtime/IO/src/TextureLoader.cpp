#include "TextureLoader.h"
#include "Texture.h"
#include "TextureDriver.h"
#include "YamlSerializer.h"
#include "stb_image.h"

using namespace LindaEngine;

void TextureLoader::Load(Ref<Texture> texture)
{
    switch (texture->type)
    {
    case TextureType::Tex2D: LoadTexture2D(texture); break;
    case TextureType::Cube: LoadCubemap(texture); break;
    }
}

Ref<Texture> TextureLoader::Load(std::string& path)
{
    Ref<Texture> texture = CreateRef<Texture2D>();
    texture->width = 1;
    texture->height = 1;
    texture->path = path;

    uint32_t data;

    //数据要反过来，大多数系统是小端序
    if (path == "white")
        data = 0xFFFFFFFF;
    else if (path == "black")
        data = 0xFF000000;
    else if (path == "gray")
        data = 0xFF808080;
    else if (path == "bump")
        data = 0xFFFF8080;

    TextureDriver::CreateTexture2D(texture, &data, 4, 8);
    return texture;
}

void TextureLoader::Delete(Ref<Texture> texture)
{
    TextureDriver::DeleteTexture(texture);
}

TextureLoader::TextureTempData TextureLoader::LoadToMemory(const char* path)
{
    void* data = nullptr;
    int width, height, channels, bitCount;

    if (stbi_is_16_bit(path)) {
        stbi_load_16(path, &width, &height, &channels, 0);
        bitCount = 16;
    }
    else if (stbi_is_hdr(path)) {
        stbi_loadf(path, &width, &height, &channels, 0);
        bitCount = 32;
    }
    else {
        data = stbi_load(path, &width, &height, &channels, 0);
        bitCount = 8;
    }

    TextureLoader::TextureTempData ttd;
    ttd.bitCount = bitCount;
    ttd.width = width;
    ttd.height = height;
    ttd.channels = channels;
    ttd.data = data;
    return ttd;
}

void TextureLoader::LoadTexture2D(Ref<Texture> texture)
{
    TextureLoader::TextureTempData ttd = LoadToMemory(texture->path.c_str());

    texture->width = ttd.width;
    texture->height = ttd.height;
    texture->isLoad = true;

    TextureDriver::CreateTexture2D(texture, ttd.data, ttd.channels, ttd.bitCount);

    stbi_image_free(ttd.data);
}

void TextureLoader::LoadCubemap(Ref<Texture> texture)
{
    Ref<Cubemap> cubemap = DynamicCastRef(Cubemap, texture);
    if (cubemap->srcType == CubemapSrcType::SixTexture)
    {
        TextureLoader::TextureTempData right = LoadToMemory(cubemap->right.c_str());
        TextureLoader::TextureTempData left = LoadToMemory(cubemap->left.c_str());
        TextureLoader::TextureTempData top = LoadToMemory(cubemap->top.c_str());
        TextureLoader::TextureTempData bottom = LoadToMemory(cubemap->bottom.c_str());
        TextureLoader::TextureTempData front = LoadToMemory(cubemap->front.c_str());
        TextureLoader::TextureTempData back = LoadToMemory(cubemap->back.c_str());

        texture->width = right.width;
        texture->height = right.height;
        texture->isLoad = true;

        TextureDriver::CreateCube(texture, right.data, left.data, top.data, bottom.data, front.data, back.data, right.channels, right.bitCount);

        stbi_image_free(right.data);
        stbi_image_free(left.data);
        stbi_image_free(top.data);
        stbi_image_free(bottom.data);
        stbi_image_free(front.data);
        stbi_image_free(back.data);
    }
    else
    {
        //TODO 矩形天空盒
    }

}
