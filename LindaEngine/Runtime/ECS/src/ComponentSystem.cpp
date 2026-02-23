#include "ComponentSystem.h"
#include "TransformSystem.h"
#include "CameraSystem.h"
#include "RendererSystem.h"
#include "LightSystem.h"
#include "AnimationSystem.h"

using namespace LindaEngine;

void ComponentSystem::Tick(float deltaTime)
{
	TransformSystem::Tick(deltaTime);
	AnimationSystem::Tick(deltaTime);
	CameraSystem::Tick(deltaTime);
	LightSystem::Tick(deltaTime);
	RendererSystem::Tick(deltaTime);
}

void ComponentSystem::OnDeserializeFinish()
{
	TransformSystem::OnDeserializeFinish();
	AnimationSystem::OnDeserializeFinish();
	CameraSystem::OnDeserializeFinish();
	LightSystem::OnDeserializeFinish();
	RendererSystem::OnDeserializeFinish();
}

void ComponentSystem::Finalize()
{
	RendererSystem::Clear();
	LightSystem::Clear();
	CameraSystem::Clear();
	AnimationSystem::Clear();
	TransformSystem::Clear();
}
