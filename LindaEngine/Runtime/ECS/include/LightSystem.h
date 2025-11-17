#pragma once

#include <vector>
#include <string>

namespace LindaEngine
{
	class Light;

	class LightSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Light* light);
		static void Remove(Light* light);
		static void Clear();

	private:
		static std::vector<Light*> _components;
	};
}
