#pragma once

#include "RenderPipeline.h"
#include "UniversalRendererEditor.h"

namespace LindaEngine
{
	class Camera;
	class Entity;
}

namespace LindaEditor
{

	class RenderPipelineEditor : public LindaEngine::RenderPipeline
	{
	public:
		virtual int Initialize();
		virtual void Finalize();

	protected:
		void Render();

	private:
		UniversalRendererEditor _urpEditor;
		Ref<LindaEngine::Entity> _entity = nullptr;

	public:
		static LindaEngine::Camera* activeCamera;
	};
}

