#include "Camera.h"
#include "Core/Transform.h"
#include "Core/EventCode.h"

using namespace LindaEngine;

Camera::Camera()
{
	Bind(EventCode::TransformChange);
	std::cout << "		Camera" << std::endl;
}

Camera::~Camera()
{
	std::cout << "		~Camera" << std::endl;
}

const glm::mat4& Camera::GetViewMatrix()
{
	Transform* go = _entity->GetTransform();

	glm::vec3 pos = go->GetWorldPosition();
	glm::quat rotation = go->GetWorldRotation();

	_viewMatrix = glm::mat4_cast(rotation);

	_viewMatrix[3][0] = pos.x;
	_viewMatrix[3][1] = pos.y;
	_viewMatrix[3][2] = pos.z;
	_viewMatrix[3][3] = 1.0f;

	_viewMatrix = glm::inverse(_viewMatrix);

	return _viewMatrix;
}

const glm::mat4& Camera::GetProjectMatrix()
{
	return NULL;
}

const glm::mat4& Camera::GetViewProjectMatrix()
{
	return NULL;
}

void Camera::OnEvent(void* userData)
{
	std::cout << "Camera::OnEvent" << std::endl;
}

void Camera::Tick()
{
}
