#include "CameraSystem.h"
#include "Camera.h"
#include "Entity.h"

using namespace LindaEngine;

std::vector<Camera*> CameraSystem::_components;
Camera* CameraSystem::currentCamear;

void CameraSystem::Tick()
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

void CameraSystem::Add(Camera* camera)
{
	_components.push_back(camera);
}

void CameraSystem::Remove(Camera* camera)
{
	auto itr = std::find(_components.begin(), _components.end(), camera);
	if (itr != _components.end())
		_components.erase(itr);
}

void CameraSystem::Clear()
{
	if (false == _components.empty())
		static_assert(true, "CameraSystem is not empty, Check destruction process.");

	_components.clear();
}

const std::vector<Camera*> CameraSystem::GetActiveCameraList()
{
	std::vector<Camera*> list;

	for (auto& com : _components) {
		if (com->IsEnable() == false)
			continue;
		list.push_back(com);
	}

	return list;
}
