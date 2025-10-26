#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.hpp"

namespace LindaEngine
{
	class Scene;
	class RenderPipeline : public IRuntimeModule
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

		static Scope<RenderPipeline> Create();

	private:
		void CheckCameraList();
		void CheckLightList();
		void Render();

		void test();
	};
}
