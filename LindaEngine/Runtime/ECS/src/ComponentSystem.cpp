#include "ComponentSystem.h"
#include "TransformSystem.h"
#include "CameraSystem.h"
#include "RendererSystem.h"

using namespace LindaEngine;

void ComponentSystem::Tick()
{
	TransformSystem::Tick();
	CameraSystem::Tick();
	RendererSystem::Tick();
}

void ComponentSystem::OnDeserializeFinish()
{
	TransformSystem::OnDeserializeFinish();
	CameraSystem::OnDeserializeFinish();
	RendererSystem::OnDeserializeFinish();
}

void ComponentSystem::Clear()
{
	TransformSystem::Clear();
	CameraSystem::Clear();
	RendererSystem::Clear();
}
