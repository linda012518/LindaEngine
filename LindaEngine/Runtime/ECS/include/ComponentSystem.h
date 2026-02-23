#pragma once

namespace LindaEngine
{
	class ComponentSystem
	{
	public:
		static void Tick(float deltaTime);
		static void OnDeserializeFinish();
		static void Finalize();
	};
}
