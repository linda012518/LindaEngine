#include "OutLinePostProcess.h"
#include "ClassImplement.inl"
#include "YamlSerializer.h"
#include "HierarchyPanelEditor.h"
#include "Entity.h"
#include "Renderer.h"
#include "Graphic.h"
#include "TextureManager.h"
#include "Texture.h"
#include "RendererSystem.h"
#include "Material.h"
#include "MaterialManager.h"

using namespace LindaEngine;
using namespace LindaEditor;

DYNAMIC_CREATE_CLASS(OutLinePostProcess, PostProcessEffectRenderer)

OutLinePostProcess::OutLinePostProcess()
{
	Initialize();
}

OutLinePostProcess::~OutLinePostProcess()
{
	Finalize();
}

void OutLinePostProcess::Initialize()
{
	_maskMaterial = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/PostProcess/OutLine/OutLineMask.shader");
	_edgeDetectionMaterial = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/PostProcess/OutLine/EdgeDetection.shader");
	_blurMaterial = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/PostProcess/OutLine/SeparableBlur.shader");
	_overlayMaterial = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/PostProcess/OutLine/OutLineOverlay.shader");
}

void OutLinePostProcess::Finalize()
{

}

void OutLinePostProcess::Render(Ref<RenderTexture> src, Ref<RenderTexture> dest)
{
	std::vector<Entity*>& entitys = HierarchyPanelEditor::GetSelectedEntity();
	if (entitys.size() <= 0)
	{
		Graphic::Blit(src, dest);
		return;
	}

	Ref<RenderTexture> temp = RenderTexture::active;

	int downSampleRatio = 2;
	int width = src->width;
	int height = src->height;
	int resx = width / downSampleRatio;
	int resy = height / downSampleRatio;
	//…Í«ÎRT
	FramebufferTextureSpecification fts;
	fts.colorFormat = TextureFormat::RGBA8;
	Ref<RenderTexture> maskBuffer = RenderTextureManager::Get(width, height, fts);
	Ref<RenderTexture> maskDownSampleBuffer = RenderTextureManager::Get(resx, resy, fts);
	Ref<RenderTexture> edgeBuffer1 = RenderTextureManager::Get(resx, resy, fts);
	Ref<RenderTexture> edgeBuffer2 = RenderTextureManager::Get(resx / 2, resy / 2, fts);
	Ref<RenderTexture> blurBuffer1 = RenderTextureManager::Get(resx, resy, fts);
	Ref<RenderTexture> blurBuffer2 = RenderTextureManager::Get(resx / 2, resy / 2, fts);

	RenderTextureManager::SetRenderTarget(maskBuffer);
	Graphic::SetViewport(0, 0, width, height);
	Graphic::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Graphic::Clear(true, true, true);
	for (auto& entity : entitys)
	{
		Renderer* render = entity->GetComponent<Renderer>();
		if (nullptr == render)
			continue;
		RendererSystem::DrawRenderer(render, _maskMaterial);
	}
	Graphic::Blit(maskBuffer, maskDownSampleBuffer);
	Graphic::Blit(maskDownSampleBuffer, edgeBuffer1, _edgeDetectionMaterial);

	_blurMaterial->SetVec4("direction_texelSize", glm::vec4(1.0, 0.0, resx, resy));
	_blurMaterial->SetFloat("kernelRadius", 1.0);
	_blurMaterial->SetFloat("maxRadius", 4.5);
	Graphic::Blit(edgeBuffer1, blurBuffer1, _blurMaterial);
	_blurMaterial->SetVec4("direction_texelSize", glm::vec4(0.0, 1.0, resx, resy));
	Graphic::Blit(blurBuffer1, edgeBuffer1, _blurMaterial);

	_blurMaterial->SetVec4("direction_texelSize", glm::vec4(1.0, 0.0, resx / 2, resy / 2));
	_blurMaterial->SetFloat("kernelRadius", 4.0);
	_blurMaterial->SetFloat("maxRadius", 4.5);
	Graphic::Blit(edgeBuffer1, blurBuffer2, _blurMaterial);
	_blurMaterial->SetVec4("direction_texelSize", glm::vec4(0.0, 1.0, resx / 2, resy / 2));
	Graphic::Blit(blurBuffer2, edgeBuffer2, _blurMaterial);

	_overlayMaterial->SetTexture("maskTexture", maskBuffer);
	_overlayMaterial->SetTexture("edgeTexture1", edgeBuffer1);
	_overlayMaterial->SetTexture("edgeTexture2", edgeBuffer2);
	_overlayMaterial->SetFloat("edgeStrength", 2.0);
	_overlayMaterial->SetFloat("edgeGlow", 1.0);
	Graphic::Blit(src, dest, _overlayMaterial);

	RenderTextureManager::SetRenderTarget(temp);

	RenderTextureManager::Release(maskBuffer);
	RenderTextureManager::Release(maskDownSampleBuffer);
	RenderTextureManager::Release(edgeBuffer1);
	RenderTextureManager::Release(edgeBuffer2);
	RenderTextureManager::Release(blurBuffer1);
	RenderTextureManager::Release(blurBuffer2);
}

bool OutLinePostProcess::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	out << YAML::Key << "Name" << YAML::Value << "OutLinePostProcess";
	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::EndMap;

	return true;
}

bool OutLinePostProcess::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	return true;
}