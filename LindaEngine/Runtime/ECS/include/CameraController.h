#pragma once

#include "Behavior.h"

namespace LindaEngine
{
	class CameraController : public Behavior
	{
	public:
		DECLARE_BEHAVIOR()
		DECLARE_DYNAMIC_CREATE(CameraController)
		CameraController(Entity& entity, bool enable = true);
		~CameraController();

		void Awake();
		void Update();

	};

}
