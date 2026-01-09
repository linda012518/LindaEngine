#pragma once

#include <vector>
#include <string>

namespace LindaEngine
{
	class Transform;
	class Entity;

	class TransformSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Transform* trans);
		static void Remove(Transform* trans);
		static void Clear();
		static Transform* Get(std::string& uuid);
		static Transform* Get(std::vector<Transform*>& list, std::string& uuid);

	private:
		static std::vector<Transform*> _components;
	};
}
