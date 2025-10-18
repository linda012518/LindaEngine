#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"

#include <vector>

namespace YAML { class Emitter; }

namespace LindaEngine
{
	class Entity;

	class Scene : public LObject, public ISerializable
	{
	public:
		Entity* CreateEntity(const char* name, bool active = true);
		void DestroyEntity(Entity* entity);
		Entity* GetEntity(const char* name);

		void Serialize();
		bool Deserialize();

	private:
		void DestroyEntityIncludeChild(Entity* entity);

	private:
		std::vector<Ref<Entity>> _entitys;
		const char* _path = nullptr;

	public:
		static Ref<Scene> overrideScene;
	};
}
