#pragma once

#include "Application.h"
#include "ImGuiContextEditor.h"

namespace LindaEngine
{
	class RenderPipeline;
}

namespace LindaEditor
{
	class ApplicationEditor : public LindaEngine::Application
	{
	public:
		int Initialize();
		void Finalize();
		void Tick();

	private:
		Ref<LindaEngine::RenderPipeline> _editorRP;
		Ref<LindaEngine::RenderPipeline> _runtimeRP;
		ImGuiContextEditor _imgui;
	};
}

