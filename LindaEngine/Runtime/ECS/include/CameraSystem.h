#pragma once

#include "AutoPtr.h"
#include <vector>

namespace LindaEngine
{
	class Camera;

	class CameraSystem
	{
	public:
		static void Tick(float deltaTime);
		static void OnDeserializeFinish();

		static void Add(Weak<Camera> camera);
		static void Remove(Weak<Camera> camera);
		static void Clear();

		static const std::vector<Weak<Camera>> GetActiveCameraList();

	private:
		static std::vector<Weak<Camera>> _components;
	};
}
