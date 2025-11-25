#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.h"
#include "UniformDataGlobal.h"
#include "UniversalRenderer.h"

#include <vector>

namespace LindaEngine
{
	class Camera;

	class RenderPipeline : public IRuntimeModule
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

		static Scope<RenderPipeline> Create();

	private:
		const std::vector<Camera*> CheckCameraList();
		void SetupShaderParameters(Camera* camera);
		void SetupLightListShaderParameters(Camera* camera, UniformDataGlobal::Data& block);
		void SetupCameraShaderParameters(Camera* camera, UniformDataGlobal::Data& block);
		void Render();

	private:
		Ref<UniformDataGlobal> _uniformGlobal;
		UniversalRenderer _urp;
	};
}
