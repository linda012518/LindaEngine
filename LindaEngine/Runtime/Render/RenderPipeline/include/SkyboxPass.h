#pragma once

#include "ScriptablePass.h"

namespace LindaEngine
{
	class SkyboxPass : public ScriptablePass
	{
	public:
		SkyboxPass();
		void Render(Camera* camera);
	};
}

