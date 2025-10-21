#pragma once

#include <vector>

#include "Camera.h"

namespace LindaEngine
{
	template <typename TComponent>
	class ComponentSystem
	{
	public:
		static void Add(TComponent* com)
		{
			_components.push_back(com);
		}

		static void Remove(TComponent* com)
		{
			auto itr = std::find(_components.begin(), _components.end(), com);
			if (itr != _components.end())
				_components.erase(itr);
		}

		static void Clear()
		{
			if (false == _components.empty())
				static_assert(true, "TransformSystem is not empty, Check destruction process.");

			_components.clear();
		}

	private:
		inline static std::vector<TComponent*> _components;
	};

	class NativeSystem : public ComponentSystem<Camera>
	{

	};
}
