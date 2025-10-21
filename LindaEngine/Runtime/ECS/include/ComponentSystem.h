#pragma once

namespace LindaEngine
{
	class ComponentSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();
		static void Clear();
	};
}
