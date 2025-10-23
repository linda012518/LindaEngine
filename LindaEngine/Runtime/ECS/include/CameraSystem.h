#pragma once

#include <vector>

namespace LindaEngine
{
	class Camera;

	class CameraSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Camera* camera);
		static void Remove(Camera* camera);
		static void Clear();

		static const std::vector<Camera*> GetActiveCameraList();

	public:
		static Camera* currentCamear;

	private:
		static std::vector<Camera*> _components;
	};
}
