#include "TestAnimation.h"
#include "BehaviorImplement.inl"
#include "Entity.h"
#include "Animation.h"
#include "FBXLoader.h"

IMPLEMENT_BEHAVIOR(TestAnimation)

using namespace LindaEngine;

void TestAnimation::Start()
{
	Animation* anim = _entity.AddComponent<Animation>();
	auto clip = FBXLoader::LoadAnimationClip("Assets/Meshs/Lethe@idle.FBX");
	anim->SetClip(clip);
	anim->PlayAnimation(0);
}