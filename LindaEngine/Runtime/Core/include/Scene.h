#pragma once

#include "AutoPtr.h"
#include "LObject.h"

#include <vector>

namespace LindaEngine
{
	class Entity;

	class Scene : public LObject
	{
	public:
		Entity* CreateEntity(const char* name, bool active = true);
		void DestroyEntity(Entity* entity);

	private:
		void DestroyEntityIncludeChild(Entity* entity);

	private:
		std::vector<Ref<Entity>> _entitys;
	};
}
