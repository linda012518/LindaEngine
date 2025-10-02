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

		static void OnAwake();
		static void OnEnable();
		static void OnStart();
		static void LateUpdate();

		static void Add(Transform* trans);
		static void Remove(Transform* trans);
		static void Clear();

		static void AddToWhite(Transform* trans);
		static void AddToBlack(Transform* trans);

	private:
		static std::vector<Transform*> _components;
		static std::vector<Transform*> _blackComponents;
	};
}
