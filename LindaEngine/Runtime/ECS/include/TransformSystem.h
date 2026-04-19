#pragma once

#include "AutoPtr.h"
#include <vector>
#include <string>

namespace LindaEngine
{
	class Transform;
	class Entity;

	class TransformSystem
	{
	public:
		static void Tick(float deltaTime);
		static void OnDeserializeFinish();

		static void Add(Weak<Transform> trans);
		static void Remove(Weak<Transform> trans);
		static void Clear();
		static Weak<Transform> Get(std::string& uuid);
		static Weak<Transform> Get(std::vector<Weak<Transform>>& list, std::string& uuid);

	private:
		static std::vector<Weak<Transform>> _components;
	};
}
