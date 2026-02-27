#pragma once

#include "Component.h"
#include "AnimationClip.h"

#include <vector>
#include <unordered_map>

namespace LindaEngine
{
	// 组件添加在根骨骼
	class Animation : public Component
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		Animation(Entity& entity, bool enable = true);
		virtual ~Animation();

		void Tick();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		void PlayAnimation(int index);
		void PlayAnimation(std::string name);
		void PlayAnimation(Ref<AnimationClip> clip);

		void SetClip(Ref<AnimationClip> clip);
		void DeleteClip(Ref<AnimationClip> clip);
		void DeleteClip(int index);
		void DeleteClip(std::string name);
		Ref<AnimationClip> GetClip(int index);
		Ref<AnimationClip> GetClip(std::string name);

		void TickAnimation(float deltaTime);

	private:
		void UpdateBoneMap();

	private:
		std::vector<Ref<AnimationClip>> _clips;
		float _currentTime = 0.0f;
		Ref<AnimationClip> _currentClip;
		std::unordered_map<std::string, Transform*> _boneMap;
	};
}
