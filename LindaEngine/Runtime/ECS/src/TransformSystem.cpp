#include "TransformSystem.h"
#include "Transform.h"
#include "Entity.h"

using namespace LindaEngine;

std::vector<Weak<Transform>> TransformSystem::_components;

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

void TransformSystem::Add(Weak<Transform> trans)
{
	trans->Initialize();
	_components.push_back(trans);
}

void TransformSystem::Remove(Weak<Transform> trans)
{
	auto itr = std::find(_components.begin(), _components.end(), trans);
	if (itr != _components.end())
		_components.erase(itr);
}

void TransformSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "TransformSystem is not empty, Check destruction process.");

	std::vector<Weak<Transform>> temp;

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

Weak<Transform> TransformSystem::Get(std::string& uuid)
{
	return Get(_components, uuid);
}

Weak<Transform> TransformSystem::Get(std::vector<Weak<Transform>>& list, std::string& uuid)
{
	for (auto& com : list) {
		if (com->GetEntity().GetUUID() != uuid)
			continue;
		return com;
	}
	return nullptr;
}

