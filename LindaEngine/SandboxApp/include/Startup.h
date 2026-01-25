#pragma once

#include "Behavior.h"

class Startup : public LindaEngine::Behavior
{
public:
	DECLARE_BEHAVIOR(Startup)

	void Awake();
	void Update();
};

