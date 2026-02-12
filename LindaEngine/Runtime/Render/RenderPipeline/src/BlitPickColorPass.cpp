#include "BlitPickColorPass.h"
#include "Texture.h"
#include "Graphic.h"
#include "EditViewPanelEditor.h"
#include "MaterialManager.h"
#include "Material.h"

using namespace LindaEngine;
using namespace LindaEditor;

BlitPickColorPass::BlitPickColorPass()
{
	_renderPassEvent = RenderPassEvent::AfterRenderingTransparents;
}

void BlitPickColorPass::Render(Camera* camera)
{
	Material::overrideLightMode = "BlitPickColor";
	Ref<RenderTexture> src = RenderTexture::active;
	Ref<Material> material = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/CopyIntColor.shader");
	Graphic::Blit(src, EditViewPanelEditor::pickRT, material, 0, 1);
	RenderTexture::active = src;
}