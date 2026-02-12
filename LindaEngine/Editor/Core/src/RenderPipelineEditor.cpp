#include "RenderPipelineEditor.h"
#include "TextureManager.h"
#include "Graphic.h"
#include "GraphicsContext.h"
#include "Texture.h"
#include "Camera.h"
#include "Entity.h"
#include "Transform.h"
#include "LightSystem.h"
#include "Light.h"
#include "CameraController.h"

using namespace LindaEditor;
using namespace LindaEngine;

int RenderPipelineEditor::Initialize()
{
    _entity = CreateRef<Entity>("EditorModeMainCamera");
    _entity->SetDontDestory(true);
    _activeCamera = _entity->AddComponent<PerspectiveCamera>();
    _entity->AddComponent<OrthoCamera>();
    _entity->AddComponent<CameraController>();

    RenderPipeline::Initialize();
    _urpEditor.Initialize();
    return 0;
}

void RenderPipelineEditor::Finalize()
{
    RenderPipeline::Finalize();
    _urpEditor.Finalize();
}

void RenderPipelineEditor::Tick()
{
    Render();
}

void RenderPipelineEditor::SetupShaderParameters(Camera* camera)
{
    UniformDataGlobal::Data& block = _uniformGlobal->data;

    SetupCameraShaderParameters(camera, block);
    SetupLightListShaderParameters(camera, block);
    block.time = glm::vec4(1.0);

    _uniformGlobal->SetUniformBufferData();
}

void RenderPipelineEditor::SetupLightListShaderParameters(Camera* camera, UniformDataGlobal::Data& block)
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

void RenderPipelineEditor::SetupCameraShaderParameters(Camera* camera, UniformDataGlobal::Data& block)
{
    block.view = camera->GetViewMatrix();
    block.project = camera->GetProjectMatrix();
    block.viewProject = camera->GetViewProjectMatrix();
    block.viewInverse = camera->GetViewInverseMatrix();
    block.projectInverse = camera->GetProjectInverseMatrix();
    block.viewProjectInverse = camera->GetViewProjectInverseMatrix();
    block.cameraPosition = glm::vec4(camera->GetTransform()->GetWorldPosition(), 1.0f);
}

void RenderPipelineEditor::Render()
{
    Camera* camera = _activeCamera;

    camera->SetRenderTarget(RenderTexture::finalRT);
    SetupShaderParameters(camera);

    int width = RenderTexture::finalRT->width;
    int height = RenderTexture::finalRT->height;
        
    RenderTextureManager::SetRenderTarget(RenderTexture::finalRT);

    Graphic::SetViewport(0, 0, width, height);
    Graphic::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Graphic::Clear(true, true, true);

    _urpEditor.Render(_activeCamera);

}

