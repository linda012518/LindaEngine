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
	public:
		Entity* CreateEntity(const char* name, bool active = true);
		void DestroyEntity(Entity* entity);
		Entity* GetEntity(const char* name);
		void SetIndex(int index) { _index = index; }
		std::string& GetPath() { return _path; }
		void Destroy();
		void SetSkyboxMaterial(Ref<Material> material);

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	private:
		void DestroyEntityIncludeChild(Entity* entity);

	private:
		std::vector<Ref<Entity>> _entitys;
		std::string _path = "";
		int _index = -1;
	};
}
