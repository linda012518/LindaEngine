#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.h"

#include <vector>

namespace LindaEngine
{
	struct UniformDataGlobal;
	class Camera;
	//class UniversalRenderer;

	class RenderPipeline : public IRuntimeModule
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

		static Scope<RenderPipeline> Create();

	private:
		const std::vector<Camera*> CheckCameraList();
		void CheckLightList();
		void Render();

	private:
		Ref<UniformDataGlobal> _uniformGlobal;
		//Ref<UniversalRenderer> _renderer;
	};
}
