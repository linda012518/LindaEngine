#include "RenderPipelineEditor.h"
#include "TextureManager.h"
#include "Graphic.h"
#include "GraphicsContext.h"

using namespace LindaEditor;
using namespace LindaEngine;

int RenderPipelineEditor::Initialize()
{
    RenderPipeline::Initialize();
    _imgui.Initialize();

    return 0;
}

void RenderPipelineEditor::Finalize()
{
    RenderPipeline::Finalize();
    _imgui.Finalize();
}

void RenderPipelineEditor::Tick()
{
    _imgui.Begin();
    _imgui.OnImGuiRender();
    _imgui.End();

    ////RenderTextureManager::SetRenderTarget(nullptr);
    //GraphicsConfig& config = GraphicsContext::graphicsConfig;
    //Graphic::SetViewport(0, 0, config.screenNewWidth, config.screenNewHeight);
    //Graphic::SetClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    //Graphic::Clear(true, true, true);

}

