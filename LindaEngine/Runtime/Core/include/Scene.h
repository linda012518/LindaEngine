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
		Entity* InstantiateCube(Transform* parent = nullptr);
		Entity* InstantiateSphere(Transform* parent = nullptr);
		Entity* InstantiatePlane(Transform* parent = nullptr);
		Entity* InstantiatePrefab(std::string path, Transform* parent = nullptr);
		Entity* InstantiateFBX(std::string path, Transform* parent = nullptr);
		void DestroyEntity(Entity* entity); 
		void DestroyEntityImmediately(Entity* entity);
		Entity* GetEntity(const char* name);
		Entity* GetEntity(int id);
		void SetIndex(int index) { _index = index; }
		std::string& GetPath() { return _path; }
		void SetPath(std::string path) { _path = path; }
		void Destroy();
		void SetSkyboxMaterial(Ref<Material> material);
		std::vector<Ref<Entity>>& GetEntitys() { return _entitys; }

		void ResetSceneUUID();
		void DuplicateEntity(Entity* entity);

		bool Serialize();
		bool Deserialize(YAML::Node& node);
		bool SerializePrefab(std::string path, Entity* entity);
		Entity* DeserializePrefab(std::string path, Transform* parent = nullptr);

	private:
		void ResetUUID(std::vector<Ref<Entity>>& entitys);
		void SerializeHierarchyPrefab(Entity* entity);
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
