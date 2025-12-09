#pragma once

#include "Event.h"

namespace LindaEngine
{
	class Entity;
}

namespace LindaEditor
{
	struct SwitchSelectEntityEditor : public LindaEngine::Event
	{
		LindaEngine::Entity* selectionEntity = nullptr;
	};
}
