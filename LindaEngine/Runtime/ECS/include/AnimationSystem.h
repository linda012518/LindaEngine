#pragma once

#include "Animation.h"

namespace LindaEngine
{
	class AnimationSystem
	{
	public:
		static void Tick(float deltaTime);
		static void OnDeserializeFinish();

		static void Add(Animation* anim);
		static void Remove(Animation* anim);
		static void Clear();

	private:
		static std::vector<Animation*> _components;
	};
}
