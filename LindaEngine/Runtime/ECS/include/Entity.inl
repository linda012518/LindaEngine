#pragma once

#include "Entity.h"

namespace LindaEngine
{
	template <typename TComponent, typename ... Args>
	TComponent* Entity::AddComponent(Args&& ... args)
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "T should derive from Component");
		static_assert(!std::is_same<Transform, TComponent>::value, "You can't add a Transform from an actor");

		Ref<TComponent> c = CreateRef<TComponent>(*this, std::forward<Args>(args)...);
		_components.push_back(c);
		OnComponentAdded(c.get());
		return c.get();
	}

	template <typename TComponent>
	TComponent* Entity::GetComponent()
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "T should derive from Component");

		Ref<TComponent> result(nullptr);
		for (auto it = _components.begin(); it != _components.end(); ++it)
		{
			result = DynamicCastRef(TComponent, *it);
			if (result)
			{
				return result.get();
			}
		}
		return nullptr;
	}

	template <typename TComponent>
	void Entity::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "T should derive from Component");
		static_assert(!std::is_same<Transform, TComponent>::value, "You can't remove a Transform from an actor");

		Ref<TComponent> result(nullptr);

		for (auto it = _components.begin(); it != _components.end(); ++it)
		{
			result = DynamicCastRef(TComponent, *it);
			if (result)
			{
				OnComponentRemoved(*result.get());
				_components.erase(it);
				break;
			}
		}
	}


}

