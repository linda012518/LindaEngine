#include "RenderPipeline.h"
#include "GraphicsContext.h"
#include "RendererSystem.h"
#include "Settings.h"
#include "Graphic.h"
#include "Transform.h"
#include "UniformBuffer.h"
#include "CameraSystem.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Light.h"
#include "LightSystem.h"

#include <algorithm>

using namespace LindaEngine;

int RenderPipeline::Initialize()
{
	Graphic::Initialize();
	_uniformGlobal = CreateRef<UniformDataGlobal>();

    _urp.Initialize();

    return 0;
}

void RenderPipeline::Finalize()
{
	_uniformGlobal = nullptr;
    _urp.Finalize();
}

void RenderPipeline::Tick()
{
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
    std::vector<Camera*> list = CameraSystem::GetActiveCameraList();
    std::sort(list.begin(), list.end(), [](Camera* a, Camera* b) { return a->GetDepth() < b->GetDepth(); });
    return list;
}

void RenderPipeline::SetupShaderParameters(Camera* camera)
{
    UniformDataGlobal::Data& block = _uniformGlobal->data;

    SetupCameraShaderParameters(camera, block);
    SetupLightListShaderParameters(camera, block);
    block.time = glm::vec4(1.0);

    _uniformGlobal->SetUniformBufferData();
}

void RenderPipeline::SetupLightListShaderParameters(Camera* camera, UniformDataGlobal::Data& block)
{
    //1 查找可用灯光
    //2 设置灯光参数到shader
    //3 设置阴影相机

    const std::vector<Light*> lightList = LightSystem::GetLightList(camera);
    int dirLightCount = 0;
    int pointLightCount = 0;
    int spotLightCount = 0;

    for (auto& light : lightList)
    {
        switch (light->GetLightType())
        {
        case LightType::DirectionLight:
        {
            DirectionLight* dirLight = dynamic_cast<DirectionLight*>(light);
            if (nullptr == dirLight)
                break;
            block.dirLightColors[dirLightCount] = dirLight->GetFinalColor();
            block.dirLightDirections[dirLightCount] = glm::vec4(dirLight->GetDirection(), 1.0f);
            dirLightCount++;
        }
        break;
        case LightType::SpotLight:
        {
            SpotLight* spotLight = dynamic_cast<SpotLight*>(light);
            if (nullptr == spotLight)
                break;
            block.spotLightColors[spotLightCount] = spotLight->GetFinalColor();
            block.spotLightDirections[spotLightCount] = glm::vec4(spotLight->GetDirection(), 1.0f);
            block.spotLightPositions[spotLightCount] = glm::vec4(spotLight->GetPosition(), 1.0f);

            float innerCos = glm::cos(glm::radians(0.5f * spotLight->GetInnerAngle()));
            float outerCos = glm::cos(glm::radians(0.5f * spotLight->GetOuterAngle()));
            float angleRangeInv = 1.0f / glm::max(innerCos - outerCos, 0.001f);
            block.spotLightAngles[spotLightCount] = glm::vec4(angleRangeInv, -outerCos * angleRangeInv, 0.0f, 0.0f);
            spotLightCount++;
        }
        break;
        case LightType::PointLight:
        {
            PointLight* pointLight = dynamic_cast<PointLight*>(light);
            if (nullptr == pointLight)
                break;
            float atten = 1.0f / glm::max(pointLight->GetRange() * pointLight->GetRange(), 0.00001f);
            block.pointLightColors[pointLightCount] = pointLight->GetFinalColor();
            block.pointLightColors[pointLightCount] = glm::vec4(pointLight->GetPosition(), atten);
            pointLightCount++;
        }
        break;
        }
    }
    block.lightCount.x = dirLightCount;
    block.lightCount.y = pointLightCount;
    block.lightCount.z = spotLightCount;
}

void RenderPipeline::SetupCameraShaderParameters(Camera* camera, UniformDataGlobal::Data& block)
{
    block.view = camera->GetViewMatrix();
    block.project = camera->GetProjectMatrix();
    block.viewProject = camera->GetViewProjectMatrix();
    block.viewInverse = camera->GetViewInverseMatrix();
    block.projectInverse = camera->GetProjectInverseMatrix();
    block.viewProjectInverse = camera->GetViewProjectInverseMatrix();
    block.cameraPosition = glm::vec4(camera->GetTransform()->GetWorldPosition(), 1.0f);
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
        Camera::currentRenderCamera = camera;

        SetupShaderParameters(camera);

        GraphicsConfig& config = GraphicsContext::graphicsConfig;
        int width = config.screenNewWidth;
        int height = config.screenNewHeight;
        Ref<RenderTexture> rt = camera->GetRenderTarget();
        if (rt != nullptr)
        {
            width = rt->width;
            height = rt->height;
        }
        else if (camera->HasPostProcess())
        {
            std::vector<FramebufferTextureSpecification> fts;

            FramebufferTextureSpecification color;
            color.colorFormat = camera->GetHDR() ? TextureFormat::RGBA32 : TextureFormat::RGBA8;
            fts.push_back(color);
            FramebufferTextureSpecification depth;
            depth.colorFormat = TextureFormat::Depth16;
            depth.filter = FilterMode::Point;
            depth.isRenderBuffer = true;
            fts.push_back(depth);

            rt = RenderTextureManager::Get(width, height, fts, camera->GetMSAA());
        }
        RenderTextureManager::SetRenderTarget(rt);

        Graphic::SetViewport(0, 0, width, height);
        glm::vec4& color = camera->GetClearColor();
        Graphic::SetClearColor(color.r, color.g, color.b, color.a);
        switch (camera->GetClearType())
        {
        case CameraClearType::Skybox:
        case CameraClearType::SolidColor:
            Graphic::Clear(true, true, true);
            break;
        case CameraClearType::DepthOnly:
            Graphic::Clear(false, true, true);
            break;
        case CameraClearType::DontClear:
            Graphic::Clear(false, false, false);
            break;
        }

        _urp.Render(camera);
    }

    Camera::currentRenderCamera = nullptr;
}



