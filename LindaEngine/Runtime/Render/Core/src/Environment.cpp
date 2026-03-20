#include "Environment.h"
#include "Texture.h"
#include "TextureDriver.h"

using namespace LindaEngine;

Ref<Texture> Environment::_cubemap = nullptr;
Ref<Texture> Environment::_irradianceMap = nullptr;
Ref<Texture> Environment::_prefilterMap = nullptr;
Ref<Texture> Environment::_brdfLUTMap = nullptr;
SHCoefficients Environment::_sh;

void Environment::SetSkybox(Ref<Texture> cubemap)
{
    _cubemap = cubemap;
    _sh = ComputeSHFromCubemap(cubemap);

    TextureDriver::DeleteTexture(_irradianceMap);
    TextureDriver::DeleteTexture(_prefilterMap);
    TextureDriver::DeleteTexture(_brdfLUTMap);
    _irradianceMap = CreateRef<Cubemap>();
    _prefilterMap = CreateRef<Cubemap>();
    _brdfLUTMap = CreateRef<Texture2D>();

    TextureDriver::CreateIBLIrradianceMap(cubemap, _irradianceMap);
    TextureDriver::CreateIBLPrefilterMap(cubemap, _prefilterMap);
    TextureDriver::CreateIBLBRDFMap(_brdfLUTMap);
}

SHCoefficients Environment::ComputeSHFromCubemap(Ref<Texture> cubemap)
{
    return SphericalHarmonics::ComputeSHFromCubemap(TextureDriver::GetPixelByCubemap(cubemap), cubemap->width);
}

