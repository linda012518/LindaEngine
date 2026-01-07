#pragma once

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"

#include <vector>

namespace LindaEngine
{
	class Entity;
	class Material;
	class Transform;
	struct FBXResources;

	class Scene : public LObject, public ISerializable
	{
		friend class SceneManager;
	public:
		Entity* CreateEntity(const char* name, bool active = true);
		Entity* InstantiatePrefab(std::string path);
		Entity* InstantiateFBX(std::string path);
		void DestroyEntity(Entity* entity); 
		void DestroyEntityImmediately(Entity* entity);
		Entity* GetEntity(const char* name);
		void SetIndex(int index) { _index = index; }
		std::string& GetPath() { return _path; }
		void SetPath(std::string path) { _path = path; }
		void Destroy();
		void SetSkyboxMaterial(Ref<Material> material);
		std::vector<Ref<Entity>>& GetEntitys() { return _entitys; }

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		void DestroyEntity();
		void UpdateEntityComponents();
		void DestroyEntityIncludeChild(Entity* entity);
		Entity* CreateEntityFromFBX(Ref<FBXResources> res, Transform* parent);

	private:
		std::vector<Ref<Entity>> _entitys;
		std::vector<Entity*> _dirtyEntitys;
		std::string _path = "";
		int _index = -1;
	};
}
