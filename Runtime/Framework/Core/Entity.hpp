#pragma once

#include <vector>
#include <string>

#include "LObject.hpp"
#include "Transform.h"

namespace LindaEngine
{
	class Component;
	class Entity : public LObject
	{
	protected:
		std::string _name;
		Ref<Transform> _transform;

	public:
		Entity();
		Entity(const char* name);
		virtual ~Entity();

		void SetName(const std::string& name);

		const std::string& GetName() const;

		Transform* GetTransform();

		void TransformChange();

		Ref<Component> AddComponent(const std::string& className);
		template <typename TComponent, typename ... Args>
		TComponent* AddComponent(Args&& ... args)
		{
			Ref<TComponent> c = CreateRef<TComponent>(std::forward<Args>(args)...);
			c->SetEntity(this);
			c->Awake();
			_components.push_back(c);
			return c.get();
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
