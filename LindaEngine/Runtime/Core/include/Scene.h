#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"

#include <vector>

namespace LindaEngine
{
	class Entity;
	class Material;

	class Scene : public LObject, public ISerializable
	{
		friend class SceneManager;
	public:
		Entity* CreateEntity(const char* name, bool active = true);
		void DestroyEntity(Entity* entity); 
		void DestroyEntityImmediately(Entity* entity);
		Entity* GetEntity(const char* name);
		void SetIndex(int index) { _index = index; }
		std::string& GetPath() { return _path; }
		void Destroy();
		void SetSkyboxMaterial(Ref<Material> material);
		std::vector<Ref<Entity>>& GetEntitys() { return _entitys; }

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		void DestroyEntity();
		void UpdateEntityComponents();
		void DestroyEntityIncludeChild(Entity* entity);

	private:
		std::vector<Ref<Entity>> _entitys;
		std::vector<Entity*> _dirtyEntitys;
		std::string _path = "";
		int _index = -1;
	};
}
