#include "LightSystem.h"
#include "Light.h"
#include "Entity.h"
#include "Camera.h"
#include "CullTool.h"

using namespace LindaEngine;

std::vector<Light*> LightSystem::_components;

void LightSystem::Tick()
{
	for (auto& com : _components) {
		if (false == com->IsEnable())
			continue;
		com->Tick();
	}
}

void LightSystem::OnDeserializeFinish()
{
	for (auto& light : _components)
	{
		if (light->GetLightType() == LightType::DirectionLight)
			continue;
		light->CalculateAABB();
	}
}

void LightSystem::Add(Light* light)
{
	_components.push_back(light);
}

void LightSystem::Remove(Light* light)
{
	auto itr = std::find(_components.begin(), _components.end(), light);
	if (itr != _components.end())
		_components.erase(itr);
}

void LightSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "LightSystem is not empty, Check destruction process.");

	std::vector<Light*> temp;

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

const std::vector<Light*> LightSystem::GetLightList(Camera* camera)
{
	std::vector<Light*> list;

	Frustum& frustum = camera->GetFrustum();

	for (auto& light : _components)
	{
		if (light->IsEnable() == false)
			continue;
		if (light->GetLightType() == LightType::DirectionLight || CullTool::FrustumCull(frustum, light->GetBoundingBox()))
			list.push_back(light);
	}

	return list;
}

