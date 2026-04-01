#include "Environment.h"
#include "Texture.h"
#include "TextureDriver.h"
#include "Material.h"

using namespace LindaEngine;

Ref<Material> Environment::_material = nullptr;
Ref<Texture> Environment::_cubemap = nullptr;
Ref<Texture> Environment::_irradianceMap = nullptr;
Ref<Texture> Environment::_prefilterMap = nullptr;
Ref<Texture> Environment::_brdfLUTMap = nullptr;
SHCoefficients Environment::_sh;

void Environment::SetSkybox(Ref<Material> material)
{
    _material = material;
    RenderSkyboxMap();
}

void Environment::RenderSkyboxMap()
{
    TextureDriver::DeleteTexture(_cubemap);
    TextureDriver::DeleteTexture(_irradianceMap);
    TextureDriver::DeleteTexture(_prefilterMap);

    _irradianceMap = CreateRef<Cubemap>();
    _irradianceMap->isUserCreate = true;
    _irradianceMap->width = 64;
    _irradianceMap->height = 64;
    _irradianceMap->isGammaCorrection = false;
    _irradianceMap->mipmapCount = 1;
    _irradianceMap->anisotropy = 1;
    _irradianceMap->colorFormat = TextureFormat::RGB16;

    _prefilterMap = CreateRef<Cubemap>();
    _prefilterMap->isUserCreate = true;
    _prefilterMap->width = 256;
    _prefilterMap->height = 256;
    _prefilterMap->isGammaCorrection = false;
    _prefilterMap->mipmapCount = 6;
    _prefilterMap->anisotropy = 1;
    _prefilterMap->colorFormat = TextureFormat::RGB16;

    _cubemap = CreateRef<Cubemap>();
    _cubemap->isUserCreate = true;
    _cubemap->width = 512;
    _cubemap->height = 512;
    _cubemap->isGammaCorrection = false;
    _cubemap->mipmapCount = 6;
    _cubemap->anisotropy = 1;
    _cubemap->colorFormat = TextureFormat::RGB16;

    TextureDriver::CreateCubeByMaterial(_material, _cubemap);

    _sh = ComputeSHFromCubemap(_cubemap);
    TextureDriver::CreateIBLIrradianceMap(_cubemap, _irradianceMap);
    TextureDriver::CreateIBLPrefilterMap(_cubemap, _prefilterMap);
}

void Environment::ComputeBRDFLutMap()
{
    TextureDriver::DeleteTexture(_brdfLUTMap);
    _brdfLUTMap = CreateRef<Texture2D>();
    _brdfLUTMap->isUserCreate = true;
    _brdfLUTMap->width = 512;
    _brdfLUTMap->height = 512;
    _brdfLUTMap->isGammaCorrection = false;
    _brdfLUTMap->mipmapCount = 1;
    _brdfLUTMap->anisotropy = 1;
    _brdfLUTMap->colorFormat = TextureFormat::RG8;
    TextureDriver::CreateIBLBRDFMap(_brdfLUTMap);
}

Ref<Texture> Environment::GetCubemap()
{
    return _cubemap;
}

SHCoefficients Environment::ComputeSHFromCubemap(Ref<Texture> cubemap)
{
    return SphericalHarmonics::ComputeSHFromCubemap(TextureDriver::GetPixelByCubemap(cubemap), cubemap->width);
}

