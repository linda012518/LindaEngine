#pragma once

#include "Base.h"
#include "Interface/IRuntimeModule.hpp"
#include "RenderAPIContext.h"

namespace LindaEngine
{
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
	};
}
