#include "Transform.h"

using namespace LindaEngine;

Transform::Transform()
{
	std::cout << "Transform" << std::endl;
}

Transform::~Transform()
{
	std::cout << "~Transform" << std::endl;
}

const glm::vec3& Transform::GetLocalPosition() const
{
	return _localPosition;
}

void Transform::SetLocalPosition(const glm::vec3& pos)
{
	_localPosition = pos;
	CalculateWorldPosition();
	UpdateMatrix();
}

const glm::vec3& Transform::GetLocalEulerAngles() const
{
	return _localEulerAngles;
}

void Transform::SetLocalEulerAngles(const glm::vec3& angle)
{
	_localEulerAngles = angle;
	CalculateWorldEulerAngles();
	UpdateRotation();
	UpdateMatrix();
}

const glm::quat& Transform::GetLocalRotation() const
{
	return _localRotation;
}

void Transform::SetLocalRotation(const glm::quat& rotation)
{
	_localRotation = rotation;
	CalculateWorldRotation();
	UpdateEulerAngles();
	UpdateMatrix();
}

const glm::vec3& Transform::GetLocalScale() const
{
	return _localScale;
}

void Transform::SetLocalScale(const glm::vec3& scale)
{
	_localScale = scale;
	CalculateWorldScale();
	UpdateMatrix();
}

const glm::vec3& Transform::GetWorldPosition() const
{
	return _position;
}

void Transform::SetWorldPosition(const glm::vec3& pos)
{
	_position = pos;
	CalculateLocalPosition();
	UpdateMatrix();
}

const glm::vec3& Transform::GetWorldEulerAngles() const
{
	return _eulerAngles;
}

void Transform::SetWorldEulerAngles(const glm::vec3& angle)
{
	_eulerAngles = angle;
	CalculateLocalEulerAngles();
	UpdateRotation();
	UpdateMatrix();
}

const glm::quat& Transform::GetWorldRotation() const
{
	return _rotation;
}

void Transform::SetWorldRotation(const glm::quat& rotation)
{
	_rotation = rotation;
	CalculateLocalRotation();
	UpdateEulerAngles();
	UpdateMatrix();
}

const glm::vec3& Transform::GetWorldScale() const
{
	return _scale;
}

void Transform::SetWorldScale(const glm::vec3& scale)
{
	_scale = scale;
	CalculateLocalScale();
	UpdateMatrix();
}

void Transform::CalculateLocalPosition()
{
}
void Transform::CalculateWorldPosition()
{

}
void Transform::CalculateLocalEulerAngles()
{

}
void Transform::CalculateWorldEulerAngles()
{

}
void Transform::CalculateLocalRotation()
{

}
void Transform::CalculateWorldRotation()
{

}
void Transform::CalculateLocalScale()
{

}
void Transform::CalculateWorldScale()
{

}
void Transform::UpdateEulerAngles()
{

}
void Transform::UpdateRotation()
{

}

void Transform::UpdateMatrix()
{
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), _position) * glm::mat4_cast(_rotation) * glm::scale(glm::mat4(1.0f), _scale);
	glm::mat4 model = glm::mat4x4(1.0f);
	model[0][0] = _scale.x;
	model[1][1] = _scale.y;
	model[2][2] = _scale.z;
	model = glm::mat4_cast(_rotation) * model;
	model[3][0] = _position.x;
	model[3][1] = _position.y;
	model[3][2] = _position.z;

	_localToWorldMatrix = model;
	_worldToLocalMatrix = glm::inverse(model);
}
