#pragma once

#include "RenderPipeline.h"
//#include "ImGuiContextEditor.h"

namespace LindaEditor
{
	class RenderPipelineEditor : public LindaEngine::RenderPipeline
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

	private:
		//ImGuiContextEditor _imgui;

	};
}

