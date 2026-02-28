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

	protected:
		void Render();

	private:
		UniversalRenderer _urp;
	};
}

