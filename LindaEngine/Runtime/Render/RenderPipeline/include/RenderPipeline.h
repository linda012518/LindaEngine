#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.h"
#include "UniformDataGlobal.h"

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

		static Ref<RenderPipeline> Create();

	protected:
		const std::vector<Camera*> CheckCameraList();
		void SetupShaderParameters(Camera* camera);
		void SetupLightListShaderParameters(Camera* camera, UniformDataGlobal::Data& block);
		void SetupCameraShaderParameters(Camera* camera, UniformDataGlobal::Data& block);

		virtual void Render() = 0;

	protected:
		static Ref<UniformDataGlobal> _uniformGlobal;
	};
}
