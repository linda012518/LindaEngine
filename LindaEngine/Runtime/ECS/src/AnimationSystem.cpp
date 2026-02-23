#include "AnimationSystem.h"
#include "Entity.h"

using namespace LindaEngine;

std::vector<Animation*> AnimationSystem::_components;

void AnimationSystem::Tick(float deltaTime)
{
	for (auto& com : _components) {
		if (false == com->IsEnable())
			continue;
		com->TickAnimation(deltaTime);
	}
}

void AnimationSystem::OnDeserializeFinish()
{
	for (auto& anim : _components)
	{

	}
}

void AnimationSystem::Add(Animation* light)
{
	_components.push_back(light);
}

void AnimationSystem::Remove(Animation* light)
{
	auto itr = std::find(_components.begin(), _components.end(), light);
	if (itr != _components.end())
		_components.erase(itr);
}

void AnimationSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "AnimationSystem is not empty, Check destruction process.");

	std::vector<Animation*> temp;

	for (auto& com : _components) {
		if (false == com->GetEntity().GetDontDestory())
			continue;
		temp.push_back(com);
	}

	_components.clear();

	for (auto& com : temp) {
		_components.push_back(com);
	}
}
