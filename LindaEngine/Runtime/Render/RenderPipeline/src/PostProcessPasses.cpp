#include "PostProcessPasses.h"
#include "PostProcessEffectRenderer.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Graphic.h"
#include "Entity.h"
#include "AppState.h"
#include "Application.h"
#include "Path.h"
#include "UniversalRenderPipeline.h"

using namespace LindaEngine;

Ref<RenderTexture> PostProcessPass::_multipleCameraRT;

PostProcessPass::PostProcessPass()
{
	_renderPassEvent = RenderPassEvent::BeforeRenderingPostProcessing;
}

void PostProcessPass::Render(Weak<Camera> camera)
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

	if (AppModule::Editor == Application::module && camera->GetRenderTarget() == nullptr && camera->GetEntity().GetName() != Path::editorModeMainCameraName)
	{
		Ref<RenderTexture> outputRT = RenderTextureManager::Get(RenderTexture::finalRT);
		StackingCamera(camera, outputRT);
	}
	else if (AppModule::Runtime == Application::module && camera->GetRenderTarget() == nullptr)
	{
		GraphicsConfig& config = GraphicsContext::graphicsConfig;
		int width = config.screenNewWidth;
		int height = config.screenNewHeight;
		FramebufferTextureSpecification fts;
		fts.colorFormat = TextureFormat::RGBA8;
		Ref<RenderTexture> outputRT = RenderTextureManager::Get(width, height, fts);

		StackingCamera(camera, outputRT);
	}
	else
	{
		Ref<Material> material = MaterialManager::GetDefaultMaterial("BuiltInAssets/Shaders/PostProcess/LinnerToGamma.shader");
		Graphic::Blit(_source, nullptr == camera->GetRenderTarget() ? RenderTexture::finalRT : camera->GetRenderTarget(), material);
	}

	RenderTextureManager::Release(_source);
	RenderTextureManager::Release(_dest);
	if (camera->GetRenderTarget() != src)
		RenderTextureManager::Release(src);
}

void PostProcessPass::StackingCamera(Weak<Camera> camera, Ref<RenderTexture> outputRT)
{
	if (nullptr == _multipleCameraRT)
		_multipleCameraRT = RenderTextureManager::Get(outputRT);
	if (UniversalRenderPipeline::isFirstCamera)
	{
		RenderTextureManager::SetRenderTarget(_multipleCameraRT);
		glm::vec4& color = camera->GetClearColor();
		Graphic::SetClearColor(color.r, color.g, color.b, color.a);
		Graphic::Clear(true, true, true);
	}

	Ref<Material> material = MaterialManager::GetDefaultMaterial("BuiltInAssets/Shaders/PostProcess/MergeMultipleCamera.shader");
	material->SetTexture("currentTexture", _multipleCameraRT);
	float factor = 1.0f;
	switch (camera->GetClearType())
	{
	case CameraClearType::DepthOnly:
	case CameraClearType::DontClear:
		factor = 0.0f;
		break;
	}
	material->SetFloat("factor", factor);
	material->SetVec4("offsetUV", camera->GetViewport());
	Graphic::Blit(_source, outputRT, material);

	RenderTextureManager::Release(_multipleCameraRT);
	_multipleCameraRT = outputRT;

	material = MaterialManager::GetDefaultMaterial("BuiltInAssets/Shaders/CopyColor.shader");
	Graphic::Blit(_multipleCameraRT, RenderTexture::finalRT, material);
}

