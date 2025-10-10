#pragma once

#include <vector>

namespace LindaEngine
{
	class Transform;
	class Entity;

	class TransformSystem
	{
	public:
		static void Tick();

		static void Add(Transform* trans);
		static void Remove(Transform* trans);
		static void Clear();

	private:
		static std::vector<Transform*> _components;
	};
}
