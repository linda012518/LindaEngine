#include "ComponentSystem.h"
#include "TransformSystem.h"
#include "CameraSystem.h"
#include "RendererSystem.h"
#include "LightSystem.h"

using namespace LindaEngine;

void ComponentSystem::Tick()
{
	TransformSystem::Tick();
	CameraSystem::Tick();
	LightSystem::Tick();
	RendererSystem::Tick();
}

void ComponentSystem::OnDeserializeFinish()
{
	TransformSystem::OnDeserializeFinish();
	CameraSystem::OnDeserializeFinish();
	LightSystem::OnDeserializeFinish();
	RendererSystem::OnDeserializeFinish();
}

void ComponentSystem::Finalize()
{
	RendererSystem::Clear();
	LightSystem::Clear();
	CameraSystem::Clear();
	TransformSystem::Clear();
}
