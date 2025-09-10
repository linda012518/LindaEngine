#include "Camera.h"
#include "Core/Transform.h"
#include "Core/EventCode.h"

using namespace LindaEngine;

Camera::Camera(const glm::mat4& projectMat)
{
	_projectMatrix = projectMat;

	std::cout << "		Camera" << _selfID << std::endl;
}

Camera::~Camera()
{
	std::cout << "		~Camera" << _selfID << std::endl;
}

const glm::mat4& Camera::GetViewMatrix()
{
	return _viewMatrix;
}

const glm::mat4& Camera::GetProjectMatrix()
{
	return _projectMatrix;
}

const glm::mat4& Camera::GetViewProjectMatrix()
{
	return _viewProjectMatrix;
}

void Camera::Awake()
{
	UpdateMatrix();
}

void Camera::Tick()
{
	UpdateMatrix();
}

void Camera::TransformChange()
{
	UpdateMatrix();

	std::cout << "		CameraTransformChange" << std::endl;
}

void Camera::UpdateMatrix()
{
	Transform* go = _entity->GetTransform();

	glm::vec3 pos = go->GetWorldPosition();
	glm::quat rotation = go->GetWorldRotation();

	_viewInverseMatrix = glm::mat4_cast(rotation);

	_viewInverseMatrix[3][0] = pos.x;
	_viewInverseMatrix[3][1] = pos.y;
	_viewInverseMatrix[3][2] = pos.z;
	_viewInverseMatrix[3][3] = 1.0f;

	_viewMatrix = glm::inverse(_viewInverseMatrix);

	_viewProjectMatrix = _projectMatrix * _viewMatrix;

	_projectInverseMatrix = glm::inverse(_projectMatrix);
	_viewProjectInverseMatrix = glm::inverse(_viewProjectMatrix);
}
