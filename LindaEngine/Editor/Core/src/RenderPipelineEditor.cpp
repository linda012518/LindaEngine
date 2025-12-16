#include "RenderPipelineEditor.h"
#include "TextureManager.h"
#include "Graphic.h"
#include "GraphicsContext.h"
#include "Texture.h"

using namespace LindaEditor;
using namespace LindaEngine;

int RenderPipelineEditor::Initialize()
{
    RenderPipeline::Initialize();
    //_imgui.Initialize();
    return 0;
}

void RenderPipelineEditor::Finalize()
{
    RenderPipeline::Finalize();
    //_imgui.Finalize();
}

void RenderPipelineEditor::Tick()
{
    //Ref<RenderTexture> editRT = _imgui.GetEditRenderTexture();
    //Ref<RenderTexture> playRT = _imgui.GetPlayRenderTexture();

    //RenderTextureManager::SetRenderTarget(editRT);
    //Graphic::SetViewport(0, 0, editRT->width, editRT->height);
    //Graphic::SetClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    //Graphic::Clear(true, true, true);

    //RenderTextureManager::SetRenderTarget(playRT);
    //Graphic::SetViewport(0, 0, playRT->width, playRT->height);
    //Graphic::SetClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    //Graphic::Clear(true, true, true);

    //_imgui.Begin();
    //_imgui.OnImGuiRender();
    //_imgui.End();



}

