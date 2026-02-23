#include "Animation.h"
#include "ComponentImplement.inl"
#include "Entity.h"
#include "YamlSerializer.h"
#include "Transform.h"
#include "Renderer.h"
#include "glm/glm.hpp"

using namespace LindaEngine;

DYNAMIC_CREATE(Animation)

Animation::Animation(Entity& entity, bool enable) : Component(entity, enable)
{

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

void Animation::SetClip(Ref<AnimationClip> clip, int index)
{
	if (index > _clips.size() - 1)
		return;
	_clips[index] = clip;
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
	_currentTime = glm::modf(_currentTime, _currentClip->duration);

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
		_boneMap[track.name] = Transform::GetChildByName(_transform, track.name);
	}
}
