#pragma once

#include "Behavior.h"

class TestECSEvent : public LindaEngine::Behavior
{
public:
	DECLARE_BEHAVIOR(TestECSEvent)

	void Awake();
	void Update();

	void OnEvent(LindaEngine::IEventHandler* sender, int eventCode, LindaEngine::Event& eventData);
};

