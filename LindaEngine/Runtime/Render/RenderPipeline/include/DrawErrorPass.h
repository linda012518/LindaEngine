#pragma once

#include "ScriptablePass.h"

namespace LindaEngine
{
	class DrawErrorPass : public ScriptablePass
	{
	public:
		DrawErrorPass();
		void Render(Camera* camera);
	};
}
