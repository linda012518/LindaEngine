#include "RenderPipeline.h"
#include "GraphicsContext.h"
#include "RendererSystem.h"
#include "Settings.h"
#include "Graphic.h"
#include "UniformDataGlobal.h"
#include "UniformBuffer.h"
#include "CameraSystem.h"
#include "Camera.h"
#include "UniversalRenderer.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Material.h"
#include "MaterialManager.h"

using namespace LindaEngine;

int RenderPipeline::Initialize()
{
	Graphic::Initialize();
	_uniformGlobal = CreateRef<UniformDataGlobal>();
    return 0;
}

void RenderPipeline::Finalize()
{
	_uniformGlobal = nullptr;
}

void RenderPipeline::Tick()
{
    //_renderer->Render();
    Render();
}

Scope<RenderPipeline> RenderPipeline::Create()
{
    return CreateScope<RenderPipeline>();
}

const std::vector<Camera*> RenderPipeline::CheckCameraList()
{
    //1 查找可用阴影相机
    //2 查找可用颜色相机
    //3 相机排序
    return CameraSystem::GetActiveCameraList();
}

void RenderPipeline::CheckLightList()
{
    //1 查找可用灯光
    //2 设置灯光参数到shader
    //3 设置阴影相机
}

void RenderPipeline::Render()
{
    //1 遍历相机
    //4 视锥剔除，排序
    //2 设置相机参数到shader
    //3 配置相机 HDR 后处理等等
    //5 设置清屏相关
    //6 渲染不透明物体
    //7 渲染天空
    //8 渲染透明物体

    const std::vector<Camera*> cameraList = CheckCameraList();

    for (auto& camera : cameraList)
    {
        UniformDataGlobal::Data& block = _uniformGlobal->data;
        block.view = camera->GetViewMatrix();
        block.project = camera->GetProjectMatrix();
        block.viewProject = camera->GetViewProjectMatrix();
        block.viewInverse = camera->GetViewInverseMatrix();
        block.projectInverse = camera->GetProjectInverseMatrix();
        block.viewProjectInverse = camera->GetViewProjectInverseMatrix();
        block.time = glm::vec4(1.0);
        _uniformGlobal->SetUniformBufferData();

        GraphicsConfig& config = GraphicsContext::graphicsConfig;

        Ref<DrawingSettings> settings = CreateRef<DrawingSettings>();
        settings->lightModes.push_back("customLightMode");

        std::vector<FramebufferTextureSpecification> fts;

        FramebufferTextureSpecification color;
        color.colorFormat = TextureFormat::RGBA8;
        fts.push_back(color);
        FramebufferTextureSpecification depth;
        depth.colorFormat = TextureFormat::Depth16;
        depth.isRenderBuffer = true;
        fts.push_back(depth);

        Ref<RenderTexture> rt = RenderTextureManager::Get(config.screenNewWidth, config.screenNewHeight, fts, 4);
        RenderTextureManager::SetRenderTarget(rt);
        Graphic::SetViewport(0, 0, config.screenNewWidth, config.screenNewHeight);
        Graphic::SetClearColor(0.0f, 0.3f, 0.0f, 0.0f);
        Graphic::Clear(true, true, true);
        RendererSystem::DrawRenderers(camera, settings);
        RendererSystem::DrawSkybox();


        FramebufferTextureSpecification gray;
        gray.colorFormat = TextureFormat::RGBA8;
        Ref<RenderTexture> grayRT = RenderTextureManager::Get(config.screenNewWidth, config.screenNewHeight, gray);
        Ref<Material> grayMaterial = MaterialManager::GetMaterialByShader("Assets/Shaders/gray.shader");
        Graphic::Blit(rt, grayRT, grayMaterial);

        Ref<Material> material = MaterialManager::GetMaterialByShader("Assets/Shaders/postprocess.shader");
        Graphic::Blit(grayRT, nullptr, material);

        RenderTextureManager::Release(rt);
        RenderTextureManager::Release(grayRT);
    }

}

