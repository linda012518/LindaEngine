#include "RenderPipelineEditor.h"
#include "TextureManager.h"
#include "Graphic.h"
#include "GraphicsContext.h"
#include "Texture.h"
#include "Camera.h"
#include "Entity.h"
#include "CameraController.h"

using namespace LindaEditor;
using namespace LindaEngine;

LindaEngine::Camera* RenderPipelineEditor::activeCamera = nullptr;

int RenderPipelineEditor::Initialize()
{
    _entity = CreateRef<Entity>("EditorModeMainCamera");
    _entity->SetDontDestory(true);
    activeCamera = _entity->AddComponent<PerspectiveCamera>();
    activeCamera->AddPostProcess("OutLinePostProcess");
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

void RenderPipelineEditor::Render()
{
    Camera* camera = activeCamera;

    camera->SetRenderTarget(RenderTexture::finalRT);
    SetupShaderParameters(camera);

    int width = RenderTexture::finalRT->width;
    int height = RenderTexture::finalRT->height;
        
    RenderTextureManager::SetRenderTarget(RenderTexture::finalRT);

    Graphic::SetViewport(0, 0, width, height);
    Graphic::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Graphic::Clear(true, true, true);

    _urpEditor.Render(activeCamera);

}

