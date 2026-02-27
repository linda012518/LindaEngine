#include "Animation.h"
#include "ComponentImplement.inl"
#include "Entity.h"
#include "YamlSerializer.h"
#include "Transform.h"
#include "Renderer.h"
#include "AnimationSystem.h"
#include "glm/glm.hpp"

using namespace LindaEngine;

DYNAMIC_CREATE(Animation)

Animation::Animation(Entity& entity, bool enable) : Component(entity, enable)
{
	AnimationSystem::Add(this);
}

Animation::~Animation()
{

}

void Animation::Tick()
{

}

bool Animation::Serialize()
{
	return true;
}

bool Animation::Deserialize(YAML::Node& node)
{
	return true;
}

void Animation::PlayAnimation(int index)
{
	PlayAnimation(GetClip(index));
}

void Animation::PlayAnimation(std::string name)
{
	PlayAnimation(GetClip(name));
}

void Animation::PlayAnimation(Ref<AnimationClip> clip)
{
	if (nullptr == clip)
		return;
	_currentTime = 0.0f;
	_currentClip = clip;
	UpdateBoneMap();
}

void Animation::SetClip(Ref<AnimationClip> clip)
{
	_clips.push_back(clip);
}

void Animation::DeleteClip(Ref<AnimationClip> clip)
{
	for (auto itr = _clips.begin(); itr != _clips.end();)
	{
		Ref<AnimationClip> c = *itr;
		if (c != clip)
			continue;
		_clips.erase(itr);
		break;
	}
}

void Animation::DeleteClip(int index)
{
	if (index > (int)_clips.size() - 1)
		return;
	_clips.erase(_clips.begin() + index);
}

void Animation::DeleteClip(std::string name)
{
	for (auto itr = _clips.begin(); itr != _clips.end();)
	{
		Ref<AnimationClip> c = *itr;
		if (c->name != name)
			continue;
		_clips.erase(itr);
		break;
	}
}

Ref<AnimationClip> Animation::GetClip(int index)
{
	if (index > _clips.size() - 1)
		return nullptr;
	return _clips[index];
}

Ref<AnimationClip> Animation::GetClip(std::string name)
{
	for (auto& clip : _clips)
	{
		if (clip->name == name)
			return clip;
	}
	return nullptr;
}

void Animation::TickAnimation(float deltaTime)
{
	if (nullptr == _currentClip)
		return;
	_currentTime += _currentClip->ticksPerSecond * deltaTime;
	_currentTime = fmod(_currentTime, _currentClip->duration);
	//if (_currentClip->isLoop && _currentTime > _currentClip->duration)
	//{
	//	_currentTime = _currentTime - _currentClip->duration;
	//}

	for (auto& track : _currentClip->tracks)
	{
		Transform* bone = _boneMap[track.name];
		if (nullptr == bone)
			continue;
		bone->SetLocalPosition(track.TickPosition(_currentTime));
		bone->SetLocalRotation(track.TickRotation(_currentTime));
		bone->SetLocalScale(track.TickScale(_currentTime));
	}
}

void Animation::UpdateBoneMap()
{
	_boneMap.clear();

	std::vector<BoneTrack>& tracks = _currentClip->tracks;
	for (auto& track : tracks)
	{
		Transform* bone = Transform::GetChildByName(_transform, track.name);
		if (nullptr == bone)
			continue;
		_boneMap[track.name] = bone;
	}
}
