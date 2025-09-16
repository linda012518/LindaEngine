#pragma once

#include "RenderState.h"

namespace LindaEngine
{
	struct Global
	{
		static RenderState renderState;
	};

	RenderState	Global::renderState;
}
