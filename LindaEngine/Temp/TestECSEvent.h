#pragma once

#include "Behavior.h"

namespace LindaEngine
{
	class TestECSEvent : public Behavior
	{
	public:
		DECLARE_BEHAVIOR(TestECSEvent)

		void Awake();
		void Update();

		void OnEvent(IEventHandler* sender, int eventCode, Event& eventData);
	};
}

