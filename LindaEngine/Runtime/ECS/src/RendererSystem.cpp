#include "RendererSystem.h"
#include "Renderer.h"

using namespace LindaEngine;

std::vector<Renderer*> RendererSystem::_components;

void RendererSystem::Tick()
{
	for (auto& com : _components) {
		com->Tick();
	}
}

void RendererSystem::OnDeserializeFinish()
{
	//for (auto& com : _components) {

	//}
}

void RendererSystem::Add(Renderer* renerer)
{
	_components.push_back(renerer);
}

void RendererSystem::Remove(Renderer* renerer)
{
	auto itr = std::find(_components.begin(), _components.end(), renerer);
	if (itr != _components.end())
		_components.erase(itr);
}

void RendererSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "RendererSystem is not empty, Check destruction process.");

	_components.clear();
}

