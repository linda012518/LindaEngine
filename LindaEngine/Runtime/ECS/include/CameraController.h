#pragma once

#include "Behavior.h"

namespace LindaEngine
{
	class Camera;

	class CameraController : public Behavior
	{
	public:
		DECLARE_BEHAVIOR(CameraController)

		void Awake();
		void Update();

		void OnEvent(IEventHandler* sender, int eventCode, Event& eventData);

	private:
		Camera* _camera = nullptr;

	};

}
