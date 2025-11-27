#include "Texture.h"

using namespace LindaEngine;

Ref<Texture> Texture::overrideTexture = nullptr;

Ref<RenderTexture> RenderTexture::active = nullptr;
