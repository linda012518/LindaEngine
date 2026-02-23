#include "TransformSystem.h"
#include "Transform.h"
#include "Entity.h"

using namespace LindaEngine;

std::vector<Transform*> TransformSystem::_components;

void TransformSystem::Tick(float deltaTime)
{
	for (auto& com : _components) {
		if (false == com->IsEnable())
			continue;
		com->Tick();
	}
}

void TransformSystem::OnDeserializeFinish()
{
	for (auto& com : _components) {
		com->SetParent(Get(com->GetParentID()));
		com->CalculateLocalMatrix();
	}
}

void TransformSystem::Add(Transform* trans)
{
	_components.push_back(trans);
}

void TransformSystem::Remove(Transform* trans)
{
	auto itr = std::find(_components.begin(), _components.end(), trans);
	if (itr != _components.end())
		_components.erase(itr);
}

void TransformSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "TransformSystem is not empty, Check destruction process.");

	std::vector<Transform*> temp;

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

Transform* TransformSystem::Get(std::string& uuid)
{
	return Get(_components, uuid);
}

Transform* TransformSystem::Get(std::vector<Transform*>& list, std::string& uuid)
{
	for (auto& com : list) {
		if (com->GetEntity().GetUUID() != uuid)
			continue;
		return com;
	}
	return nullptr;
}

