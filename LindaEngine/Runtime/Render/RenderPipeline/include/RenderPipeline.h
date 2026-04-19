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
		static Ref<UniformDataGlobal> GetGlobalUniformData() { return _uniformGlobal; }

	protected:
		const std::vector<Weak<Camera>> CheckCameraList();
		void SetupShaderParameters(Weak<Camera> camera);
		void SetupLightListShaderParameters(Weak<Camera> camera, UniformDataGlobal::Data& block);
		void SetupCameraShaderParameters(Weak<Camera> camera, UniformDataGlobal::Data& block);

		virtual void Render() = 0;

	protected:
		static Ref<UniformDataGlobal> _uniformGlobal;
	};
}
