#pragma once

#include "Behavior.h"

namespace LindaEngine
{
	class TestECSEvent : public Behavior
	{
	public:
		DECLARE_BEHAVIOR(TestECSEvent)
		DECLARE_DYNAMIC_CREATE(TestECSEvent)

		void Awake();
		void Update();

		void OnEvent(LObject* sender, int eventCode, Event& eventData);
	};
}

