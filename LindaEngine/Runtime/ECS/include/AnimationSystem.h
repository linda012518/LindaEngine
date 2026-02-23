#pragma once

#include "Animation.h"

namespace LindaEngine
{
	class AnimationSystem
	{
	public:
		static void Tick(float deltaTime);
		static void OnDeserializeFinish();

		static void Add(Animation* light);
		static void Remove(Animation* light);
		static void Clear();

	private:
		static std::vector<Animation*> _components;
	};
}
