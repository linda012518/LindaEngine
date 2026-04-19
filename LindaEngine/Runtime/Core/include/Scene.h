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
	class SkinMeshRenderer;
	struct FBXResources;

	class Scene : public LObject, public ISerializable
	{
		friend class SceneManager;
	public:
		Weak<Entity> CreateEntity(const char* name, bool active = true);
		Weak<Entity> InstantiateCube(Weak<Transform> parent = nullptr);
		Weak<Entity> InstantiateSphere(Weak<Transform> parent = nullptr);
		Weak<Entity> InstantiatePlane(Weak<Transform> parent = nullptr);
		Weak<Entity> InstantiatePrefab(std::string path, Weak<Transform> parent = nullptr);
		Weak<Entity> InstantiateFBX(std::string path, Weak<Transform> parent = nullptr);
		void DestroyEntity(Weak<Entity> entity); 
		void DestroyEntityImmediately(Weak<Entity> entity);
		Weak<Entity> GetEntity(const char* name);
		Weak<Entity> GetEntity(int id);
		void SetIndex(int index) { _index = index; }
		std::string& GetPath() { return _path; }
		void SetPath(std::string path) { _path = path; }
		void Destroy();
		void SetSkyboxMaterial(Ref<Material> material);
		std::vector<Ref<Entity>>& GetEntitys() { return _entitys; }

		void ResetSceneUUID();
		void DuplicateEntity(Weak<Entity> entity);

		bool Serialize();
		bool Deserialize(YAML::Node& node);
		bool SerializePrefab(std::string path, Weak<Entity> entity);
		Weak<Entity> DeserializePrefab(std::string path, Weak<Transform> parent = nullptr);

	private:
		void ResetUUID(std::vector<Ref<Entity>>& entitys);
		void SerializeHierarchyPrefab(Weak<Entity> entity);
		void DestroyEntity();
		void UpdateEntityComponents();
		void DestroyEntityIncludeChild(Weak<Entity> entity);
		Weak<Entity> CreateEntityFromFBX(Ref<FBXResources> res, Weak<Transform> parent);
		void CollectBonesComponents(Weak<Entity> entity, std::vector<Weak<Transform>>& transforms, std::vector<Weak<SkinMeshRenderer>>& renderers);
		Weak<Transform> GetTransformByBoneName(std::vector<Weak<Transform>>& transforms, std::string name);

	private:
		std::vector<Ref<Entity>> _entitys;
		std::vector<Weak<Entity>> _dirtyEntitys;
		std::string _path = "";
		int _index = -1;
	};
}
