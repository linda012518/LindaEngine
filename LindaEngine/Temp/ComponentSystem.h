#pragma once

#include <vector>

namespace LindaEngine
{
	template <typename TComponent>
	class ComponentSystem
	{
	public:
		void Add(TComponent* com)
		{
			if (com->GetEntity().IsActive())
				_components.push_back(com);
			else
				_blackComponents.push_back(com);
		}

		void Remove(TComponent* com)
		{
			auto itr = std::find(_components.begin(), _components.end(), com);
			if (itr != _components.end())
				_components.erase(itr);
			else
			{
				auto itr = std::find(_blackComponents.begin(), _blackComponents.end(), com);
				if (itr != _blackComponents.end())
					_blackComponents.erase(itr);
			}
		}

		void Clear()
		{
			if (false == _components.empty())
				static_assert(true, "TransformSystem is not empty, Check destruction process.");
			if (false == _blackComponents.empty())
				static_assert(true, "TransformSystem is not empty, Check destruction process.");

			_components.clear();
			_blackComponents.clear();
		}

	private:
		std::vector<TComponent*> _components;
		std::vector<TComponent*> _blackComponents;
	};
}
