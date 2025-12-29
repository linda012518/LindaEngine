#include "RenderPipeline.h"
#include "Application.h"
#include "UniversalRenderPipeline.h"
#include "RenderPipelineEditor.h"
#include "Graphic.h"

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

Ref<RenderPipeline> RenderPipeline::Create()
{
    if (Application::module != AppModule::Editor)
        return CreateRef<UniversalRenderPipeline>();
    else
        return CreateRef<RenderPipelineEditor>();
}

