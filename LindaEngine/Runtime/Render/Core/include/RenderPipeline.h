#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.hpp"
#include "RenderAPIContext.h"

namespace LindaEngine
{
	class Scene;
	class RenderPipeline : public IRuntimeModule
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();

		void SetRenderAPIContext(Ref<RenderAPIContext> rc);

		static Scope<RenderPipeline> Create();

	private:
		void CheckCameraList();
		void CheckLightList();
		void Render();

		void test();

	private:
		Ref<RenderAPIContext> _renderContext = nullptr;
		//Ref<Scene> _scene = nullptr;
	};
}
