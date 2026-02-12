#include "BlitPickColorPass.h"
#include "Texture.h"
#include "Graphic.h"
#include "EditViewPanelEditor.h"
#include "MaterialManager.h"

using namespace LindaEngine;
using namespace LindaEditor;

BlitPickColorPass::BlitPickColorPass()
{
	_renderPassEvent = RenderPassEvent::AfterRenderingTransparents;
}

void BlitPickColorPass::Render(Camera* camera)
{
	Ref<RenderTexture> src = RenderTexture::active;
	Ref<Material> material = MaterialManager::GetMaterialByShader("Assets/Shaders/CopyColor.shader");
	Graphic::Blit(src, EditViewPanelEditor::pickRT, material, 0, 1);
	RenderTexture::active = src;
}