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
    _prefilterMap = CreateRef<Cubemap>();
    _cubemap = CreateRef<Cubemap>();

    // Render Map
    TextureDriver::CreateCubeByMaterial(_material, _cubemap);

    _sh = ComputeSHFromCubemap(_cubemap);
    TextureDriver::CreateIBLIrradianceMap(_cubemap, _irradianceMap);
    TextureDriver::CreateIBLPrefilterMap(_cubemap, _prefilterMap);
}

void Environment::ComputeBRDFLutMap()
{
    TextureDriver::DeleteTexture(_brdfLUTMap);
    _brdfLUTMap = CreateRef<Texture2D>();
    TextureDriver::CreateIBLBRDFMap(_brdfLUTMap);
}

SHCoefficients Environment::ComputeSHFromCubemap(Ref<Texture> cubemap)
{
    return SphericalHarmonics::ComputeSHFromCubemap(TextureDriver::GetPixelByCubemap(cubemap), cubemap->width);
}

