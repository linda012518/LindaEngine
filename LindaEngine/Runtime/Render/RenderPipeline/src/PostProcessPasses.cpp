#include "PostProcessPasses.h"
#include "PostProcessEffectRenderer.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Graphic.h"

using namespace LindaEngine;

PostProcessPass::PostProcessPass()
{
	_renderPassEvent = RenderPassEvent::BeforeRenderingPostProcessing;
}

void PostProcessPass::Render(Camera* camera)
{
	Ref<RenderTexture> src = RenderTexture::active;
	_source = RenderTextureManager::Get(src->width, src->height, src->attachments, 1, src->mipmapCount, src->isCube, src->isGammaCorrection, src->anisotropy);
	_dest = RenderTextureManager::Get(_source);
	RenderTextureManager::BlitRenderTexture(src, _source);

	for (auto& pass : camera->GetPostStack())
	{
		if (false == pass->IsEnable())
			continue;
		pass->Render(_source, _dest);
		Ref<RenderTexture> temp = _source;
		_source = _dest;
		_dest = temp;
	}

	Ref<Material> material = MaterialManager::GetDefaultMaterial("BuiltInAssets/Shaders/CopyColor.shader");
	Graphic::Blit(_source, nullptr == camera->GetRenderTarget() ? RenderTexture::finalRT : camera->GetRenderTarget(), material);

	RenderTextureManager::Release(_source);
	RenderTextureManager::Release(_dest);
	if (camera->GetRenderTarget() != src)
		RenderTextureManager::Release(src);
}

