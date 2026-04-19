#pragma once

#include "ScriptablePass.h"

namespace LindaEngine
{
	class BlitPickColorPass : public ScriptablePass
	{
	public:
		BlitPickColorPass();
		void Render(Weak<Camera> camera);
	};
}
