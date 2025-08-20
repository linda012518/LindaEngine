#pragma once

#include <vector>
#include <string>
#include "LObject.hpp"

namespace LindaEngine
{
	class Component;

	class Entity : public LObject
	{
	protected:
		std::string _name;

	public:
		Entity();
		Entity(const char* name);
		virtual ~Entity();

		virtual void setName(const std::string& name);

		std::string& getName();

		Ref<Component> AddComponent(const std::string& className);
		template <typename TComponent>
		Ref<TComponent> AddComponent()
		{
			Ref<TComponent> c = CreateRef<TComponent>();
			_components.push_back(c);
		}

		template <typename TComponent>
		Ref<TComponent> GetComponent()
		{
			return nullptr;
		}

		friend std::ostream& operator<<(std::ostream& out, const Entity& entity)
		{
			out << entity._name << std::endl;
			return out;
		}

	private:
		std::vector<Ref<Component>> _components;

	};
}
