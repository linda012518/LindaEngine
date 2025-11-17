#include "LightSystem.h"
#include "Light.h"
#include "Entity.h"

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

	_components.clear();
}

