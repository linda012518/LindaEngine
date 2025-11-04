#pragma once

#include "Behavior.h"

namespace LindaEngine
{
	class CameraController : public Behavior
	{
	public:
		DECLARE_BEHAVIOR(CameraController)
		DECLARE_DYNAMIC_CREATE(CameraController)

		void Awake();
		void Update();

	};

}
