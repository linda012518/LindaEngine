#pragma once

#include "AutoPtr.h"
#include <vector>
#include <string>

namespace LindaEngine
{
	class Light;
	class Camera;

	class LightSystem
	{
	public:
		static void Tick(float deltaTime);
		static void OnDeserializeFinish();

		static void Add(Weak<Light> light);
		static void Remove(Weak<Light> light);
		static void Clear();

		static const std::vector<Weak<Light>> GetLightList(Weak<Camera> camera);

	private:
		static void UpdateMainLight();

	private:
		static std::vector<Weak<Light>> _components;
	};
}
