#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include "AutoPtr.h"
#include "LObject.h"

namespace LindaEngine
{
	class Component;
	class Transform;

	class Entity : public LObject
	{
	public:
		Entity(const char* name, bool active = true);
		virtual ~Entity();

		void SetName(const std::string& name);

		const std::string& GetName() const;

		void SetActive(bool active);
		bool IsActive();

		void Destroy();

		template <typename TComponent, typename ... Args>
		TComponent* AddComponent(Args&& ... args);

		template <typename TComponent>
		TComponent* GetComponent();

		template <typename TComponent>
		void RemoveComponent();

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
		std::vector<Ref<Component>> _components;
		static bool _isPlaying;
	};
}

#include "Entity.inl"