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
		virtual void Tick();

	private:
		void SetupShaderParameters(LindaEngine::Camera* camera);
		void SetupLightListShaderParameters(LindaEngine::Camera* camera, LindaEngine::UniformDataGlobal::Data& block);
		void SetupCameraShaderParameters(LindaEngine::Camera* camera, LindaEngine::UniformDataGlobal::Data& block);
		void Render();

	private:
		UniversalRendererEditor _urpEditor;
		Ref<LindaEngine::Entity> _entity = nullptr;

	public:
		static LindaEngine::Camera* activeCamera;
	};
}

