#pragma once

#include "Entity.h"

namespace LindaEngine
{
	template <typename TComponent, typename ... Args>
	Weak<TComponent> Entity::AddComponent(Args&& ... args)
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "T should derive from Component");
		static_assert(!std::is_same<Transform, TComponent>::value, "You can't add a Transform from an actor");

		Ref<TComponent> c = CreateRef<TComponent>(*this, std::forward<Args>(args)...);
		_components.push_back(c);
		OnComponentAdded(c);
		return c;
	}

	template <typename TComponent>
	Weak<TComponent> Entity::GetComponent()
	{
		static_assert(std::is_base_of<Component, TComponent>::value, "T should derive from Component");

		Ref<TComponent> result(nullptr);
		for (auto it = _components.begin(); it != _components.end(); ++it)
		{
			result = DynamicCastRef(TComponent, *it);
			if (result)
			{
				return result;
			}
		}
		return nullptr;
	}


}

