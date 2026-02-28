#include "RenderPipeline.h"
#include "Application.h"
#include "UniversalRenderPipeline.h"
#include "RenderPipelineEditor.h"
#include "Graphic.h"
#include "Transform.h"
#include "CameraSystem.h"
#include "Camera.h"
#include "Light.h"
#include "LightSystem.h"
#include "Timestamp.h"

#include <algorithm>

using namespace LindaEngine;
using namespace LindaEditor;

Ref<UniformDataGlobal> RenderPipeline::_uniformGlobal;

int RenderPipeline::Initialize()
{
    _uniformGlobal = CreateRef<UniformDataGlobal>();
    return 0;
}

void RenderPipeline::Finalize()
{
    _uniformGlobal = nullptr;
}

void RenderPipeline::Tick()
{
    Render();
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
    double second = Timestamp::GetElapsedSecond();
    block.time = glm::vec4(second / 2.0, second, second * 2.0, second * 3.0);

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

Ref<RenderPipeline> RenderPipeline::Create()
{
    if (Application::module != AppModule::Editor)
        return CreateRef<UniversalRenderPipeline>();
    else
        return CreateRef<RenderPipelineEditor>();
}

