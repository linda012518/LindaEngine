#pragma once

#include "Common/GraphicsContext.h"

namespace LindaEngine
{
	class OpenglGraphicsContext : public GraphicsContext
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();
	};
}
