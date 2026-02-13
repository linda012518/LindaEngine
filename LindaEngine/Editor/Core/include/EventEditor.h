#pragma once

#include "Event.h"

#include <unordered_map>

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

	struct PickEntityIDEditor : public LindaEngine::Event
	{
		std::unordered_map<int, int> selectID;
	};
}
