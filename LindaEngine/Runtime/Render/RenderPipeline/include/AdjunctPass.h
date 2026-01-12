#pragma once

#include "ScriptablePass.h"

namespace LindaEngine
{
	class AdjunctPass : public ScriptablePass
	{
	public:
		AdjunctPass();
		void Render(Camera* camera);
	};
}
