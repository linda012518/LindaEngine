#include "ApplicationEditor.h"
#include "YamlSerializer.h"
#include "RenderPipeline.h"
#include "SceneManagerEditor.h"

using namespace LindaEditor;
using namespace LindaEngine;

int ApplicationEditor::Initialize()
{
    int ret = Application::Initialize();
    if (ret != 0)
        return ret;

    if ((ret = SceneManagerEditor::Initialize()) != 0)
    {
        printf("SceneManagerEditor Initialize Failed...");
        return ret;
    }

    module = AppModule::Runtime;
    _runtimeRP = RenderPipeline::Create();
    _runtimeRP->Initialize();

    module = AppModule::Editor;
    _editorRP = RenderPipeline::Create();
    _editorRP->Initialize();

    _imgui.Initialize();

    SceneManagerEditor::LoadScene("BuiltInAssets/Scenes/Default.scene");

    return ret;
}

void ApplicationEditor::Finalize()
{
    _imgui.Finalize();
    Application::Finalize();
}

void ApplicationEditor::OnPreRender()
{
    _imgui.Begin();
    _imgui.OnImGuiRender();

    _imgui.SetEditRenderTexture();
    _editorRP->Tick();
    _imgui.SetPlayRenderTexture();
    _runtimeRP->Tick();

    _imgui.End();
}

