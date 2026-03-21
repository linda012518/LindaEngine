#pragma once

#include "ScriptablePass.h"

namespace LindaEngine
{
	class PrecomputedAmbientPass : public ScriptablePass
	{
	public:
		PrecomputedAmbientPass();
		void Render(Camera* camera);
	};
}
