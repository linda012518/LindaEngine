#pragma once

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

		static void Add(Light* light);
		static void Remove(Light* light);
		static void Clear();

		static const std::vector<Light*> GetLightList(Camera* camera);

	private:
		static std::vector<Light*> _components;
	};
}
