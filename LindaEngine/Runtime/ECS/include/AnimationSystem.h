#pragma once

#include "Animation.h"

namespace LindaEngine
{
	class AnimationSystem
	{
	public:
		static void Tick(float deltaTime);
		static void OnDeserializeFinish();

		static void Add(Weak<Animation> anim);
		static void Remove(Weak<Animation> anim);
		static void Clear();

	private:
		static std::vector<Weak<Animation>> _components;
	};
}
