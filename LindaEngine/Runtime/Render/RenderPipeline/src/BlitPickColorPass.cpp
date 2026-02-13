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
	// 拾取纹理未变更，Editor 后处理会有多个纹理输出占用
	Material::isPickPass = true;
	Ref<RenderTexture> src = RenderTexture::active;
	Ref<Material> material = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/CopyIntColor.shader");
	Graphic::Blit(src, EditViewPanelEditor::pickRT, material, 0, 1);
	RenderTexture::active = src;
	Material::isPickPass = false;
}