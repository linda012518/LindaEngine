#include "TransformSystem.h"
#include "Transform.h"
#include "Entity.h"

using namespace LindaEngine;

std::vector<Transform*> TransformSystem::_components;
std::vector<Transform*> TransformSystem::_blackComponents;

void TransformSystem::Tick()
{
	for (auto& com : _blackComponents) {
		com->Tick();
	}
	for (auto& com : _components) {
		com->Tick();
	}
	for (auto& com : _components) {
		com->Update();
		com->LateUpdate();
	}
}

void TransformSystem::OnAwake()
{
	for (auto& com : _components) {
		com->OnAwake();
	}
}

void TransformSystem::OnEnable()
{
	for (auto& com : _components) {
		com->OnEnable();
	}
}

void TransformSystem::OnStart()
{
	for (auto& com : _components) {
		com->OnStart();
	}
}

void TransformSystem::LateUpdate()
{
	for (auto& com : _components) {
		com->LateUpdate();
	}
}

void TransformSystem::Add(Transform* trans)
{
	if (trans->GetEntity().IsActive())
		_components.push_back(trans);
	else
		_blackComponents.push_back(trans);
}

void TransformSystem::Remove(Transform* trans)
{
	auto itr = std::find(_components.begin(), _components.end(), trans);
	if (itr != _components.end())
		_components.erase(itr);
	else
	{
		auto itr = std::find(_blackComponents.begin(), _blackComponents.end(), trans);
		if (itr != _blackComponents.end())
			_blackComponents.erase(itr);
	}
}

void TransformSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "TransformSystem is not empty, Check destruction process.");
	if (false == _blackComponents.empty())
		static_assert(true, "TransformSystem is not empty, Check destruction process.");

	_components.clear();
	_blackComponents.clear();
}

void TransformSystem::AddToWhite(Transform* trans)
{
	auto itr = std::find(_blackComponents.begin(), _blackComponents.end(), trans);
	if (itr != _blackComponents.end())
	{
		_blackComponents.erase(itr);
		_components.push_back(trans);
	}
	else
		static_assert(true, "TransformSystem::Remove does not have this transform.");
}

void TransformSystem::AddToBlack(Transform* trans)
{
	auto itr = std::find(_components.begin(), _components.end(), trans);
	if (itr != _components.end())
	{
		_components.erase(itr);
		_blackComponents.push_back(trans);
	}
	else
		static_assert(true, "TransformSystem::Remove does not have this transform.");

}
