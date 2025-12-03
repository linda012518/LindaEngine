#pragma once

#include "RenderPipeline.h"
#include "UniformDataGlobal.h"
#include "UniversalRenderer.h"

#include <vector>

namespace LindaEngine
{
	class Camera;

	class UniversalRenderPipeline : public RenderPipeline
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

	private:
		const std::vector<Camera*> CheckCameraList();
		void SetupShaderParameters(Camera* camera);
		void SetupLightListShaderParameters(Camera* camera, UniformDataGlobal::Data& block);
		void SetupCameraShaderParameters(Camera* camera, UniformDataGlobal::Data& block);
		void Render();

	private:
		UniversalRenderer _urp;
	};
}

