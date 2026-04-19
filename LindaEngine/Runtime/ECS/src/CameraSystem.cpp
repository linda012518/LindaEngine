#include "CameraSystem.h"
#include "Camera.h"
#include "Entity.h"

using namespace LindaEngine;

std::vector<Weak<Camera>> CameraSystem::_components;

void CameraSystem::Tick(float deltaTime)
{
	for (auto& com : _components) {
		if (false == com->IsEnable())
			continue;
		com->Tick();
	}
}

void CameraSystem::OnDeserializeFinish()
{
	//for (auto& com : _components) {

	//}
}

void CameraSystem::Add(Weak<Camera> camera)
{
	camera->Initialize();
	_components.push_back(camera);
}

void CameraSystem::Remove(Weak<Camera> camera)
{
	auto itr = std::find(_components.begin(), _components.end(), camera);
	if (itr != _components.end())
		_components.erase(itr);
}

void CameraSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "CameraSystem is not empty, Check destruction process.");

	std::vector<Weak<Camera>> temp;

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

const std::vector<Weak<Camera>> CameraSystem::GetActiveCameraList()
{
	std::vector<Weak<Camera>> list;

	for (auto& com : _components) {
		if (com->GetEntity().GetName() == "EditorModeMainCamera")
			continue;
		if (com->IsEnable() == false)
			continue;
		list.push_back(com);
	}

	return list;
}
