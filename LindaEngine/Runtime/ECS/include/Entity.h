#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include "AutoPtr.h"
#include "LObject.h"
#include "ISerializable.h"

namespace LindaEngine
{
	class Component;
	class Transform;

	class Entity : public LObject, public ISerializable
	{
	public:
		Entity(const char* name, bool active = true);
		virtual ~Entity();

		void SetName(const std::string& name);
		const std::string& GetName() const;

		void SetActive(bool active);
		bool IsActive();

		void SetUUID(std::string& uuid) { _uuid = uuid; }
		std::string& GetUUID() { return _uuid; }

		void Destroy();
		void TransformDirty();

		Transform* GetTransform();

		template <typename TComponent, typename ... Args>
		TComponent* AddComponent(Args&& ... args);

		template <typename TComponent>
		TComponent* GetComponent();

		template <typename TComponent>
		void RemoveComponent();

		void Serialize();
		bool Deserialize();

		friend std::ostream& operator<<(std::ostream& out, const Entity& entity)
		{
			out << entity._name << std::endl;
			return out;
		}

	private:
		void OnComponentAdded(Component* com);
		void OnComponentRemoved(Component* com);

	private:
		std::string _name;
		bool _active;
		std::string _uuid;
		Transform* _transform;
		std::vector<Ref<Component>> _components;
		static bool _isPlaying;
	};
}

#include "Entity.inl"