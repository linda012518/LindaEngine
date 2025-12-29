#pragma once

#include "AutoPtr.h"
#include "IRuntimeModule.h"
#include "UniformDataGlobal.h"

namespace LindaEngine
{
	class RenderPipeline : public IRuntimeModule
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick() = 0;

		static Ref<RenderPipeline> Create();

	protected:
		static Ref<UniformDataGlobal> _uniformGlobal;
	};
}
