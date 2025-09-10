#include "Transform.h"
#include "Core/Entity.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/euler_angles.hpp"

#define DO_LOCALCHANGE \
if (_localChange) \
	UpdateWhenLocalChange(); \
_worldChange = true; 

#define DO_WORLDCHANGE \
if (_worldChange) \
	UpdateWhenWorldChange(); \
_localChange = true;

using namespace LindaEngine;

Transform::Transform(Entity * entity)
{
	std::cout << "	Transform" << _selfID << std::endl;
	_entity = entity;
}

Transform::~Transform()
{
	std::cout << "	~Transform" << _selfID << std::endl;
}

const Entity* Transform::GetEntity() const
{
	return _entity;
}

void Transform::SetParent(Transform* parent)
{
	if (_localChange)
		UpdateWhenLocalChange();
	_worldChange = true;

	if (_parent)
	{
		_parent->RemoveChild(this);
	}
	_parent = parent;
	_parent->AddChild(this);
}

const Transform* Transform::GetParent() const
{
	return _parent;
}

const glm::mat4& Transform::GetLocalToWorldMat() const
{
	return _worldMatrix;
}

const glm::mat4& Transform::GetWorldToLocalMat()
{
	_worldInverseMatrix = glm::inverse(_worldMatrix);
	return _worldInverseMatrix;
}

void Transform::AddChild(Transform* child)
{
	_children.push_back(child);
}

void Transform::RemoveChild(Transform* child)
{
	_children.remove(child);
}

void Transform::GetWorldDir(glm::vec3& forward, glm::vec3& up, glm::vec3& right)
{
	GetDir(_worldRotation, forward, up, right);
}

void Transform::GetLocalDir(glm::vec3& forward, glm::vec3& up, glm::vec3& right)
{
	GetDir(_localRotation, forward, up, right);
}

void Transform::GetDir(glm::quat& rotation, glm::vec3& forward, glm::vec3& up, glm::vec3& right)
{
	glm::mat4 temp = glm::mat4_cast(rotation);

	right.x = temp[0][0];
	right.y = temp[1][0];
	right.z = temp[2][0];

	up.x = temp[0][1];
	up.y = temp[1][1];
	up.z = temp[2][1];

	forward.x = temp[0][2];
	forward.y = temp[1][2];
	forward.z = temp[2][2];
}

const glm::vec3& Transform::GetLocalPosition() const
{
	return _localPosition;
}

const glm::vec3& Transform::GetLocalEulerAngles() const
{
	return _localEulerAngles;
}

const glm::quat& Transform::GetLocalRotation() const
{
	return _localRotation;
}

const glm::vec3& Transform::GetLocalScale() const
{
	return _localScale;
}

void Transform::SetLocalPosition(const glm::vec3& pos)
{
	if (_worldChange)
		UpdateWhenWorldChange();
	_localChange = true;

	_localPosition = pos;
}

void Transform::SetLocalEulerAngles(const glm::vec3& angle)
{
	if (_worldChange)
		UpdateWhenWorldChange();
	_localChange = true;

	_localEulerAngles = angle;
	_localRotation = glm::quat(angle);
}

void Transform::SetLocalRotation(const glm::quat& rotation)
{
	if (_worldChange)
		UpdateWhenWorldChange();
	_localChange = true;

	_localRotation = rotation;
	_localEulerAngles = glm::eulerAngles(rotation);
}


void Transform::SetLocalScale(const glm::vec3& scale)
{
	if (_worldChange)
		UpdateWhenWorldChange();
	_localChange = true;

	_localScale = scale;
}

const glm::vec3& Transform::GetWorldPosition() const
{
	return _worldPosition;
}

const glm::vec3& Transform::GetWorldEulerAngles() const
{
	return _worldEulerAngles;
}

const glm::quat& Transform::GetWorldRotation() const
{
	return _worldRotation;
}

const glm::vec3& Transform::GetWorldScale() const
{
	return _worldScale;
}

void Transform::SetWorldPosition(const glm::vec3& pos)
{
	if (_localChange)
		UpdateWhenLocalChange();
	_worldChange = true;

	_worldPosition = pos;
}

void Transform::SetWorldEulerAngles(const glm::vec3& angle)
{
	if (_localChange)
		UpdateWhenLocalChange();
	_worldChange = true;

	_worldEulerAngles = angle;
	_worldRotation = glm::quat(angle);
}

void Transform::SetWorldRotation(const glm::quat& rotation)
{
	if (_localChange)
		UpdateWhenLocalChange();
	_worldChange = true;

	_worldRotation = rotation;
	_worldRotation = glm::eulerAngles(rotation);
}

void Transform::SetWorldScale(const glm::vec3& scale)
{
	if (_localChange)
		UpdateWhenLocalChange();
	_worldChange = true;

	_worldScale = scale;
}

void Transform::UpdateWhenLocalChange()
{
	if (_localChange == false)
		return;
	_localChange = false;

	_localMatrix = glm::mat4(1.0);

	_localMatrix[0][0] = _localScale.x;
	_localMatrix[1][1] = _localScale.y;
	_localMatrix[2][2] = _localScale.z;

	_localMatrix = glm::mat4_cast(_localRotation) * _localMatrix;

	_localMatrix[3][0] = _localPosition.x;
	_localMatrix[3][1] = _localPosition.y;
	_localMatrix[3][2] = _localPosition.z;
	_localMatrix[3][3] = 1.0f;

	if (nullptr != _parent)
	{
		_worldMatrix = _parent->_worldMatrix * _localMatrix;
		glm::vec3 skew;
		glm::vec4 m;
		glm::decompose(_worldMatrix, _worldScale, _worldRotation, _worldPosition, skew, m);
		_worldEulerAngles = glm::eulerAngles(_worldRotation);
	}
	else
	{
		_worldMatrix = _localMatrix;
		_worldEulerAngles = _localEulerAngles;
		_worldPosition = _localPosition;
		_worldScale = _localScale;
		_worldRotation = _localRotation;
	}

	NotifyChange();
}

void Transform::UpdateWhenWorldChange()
{
	if (_worldChange == false)
		return;
	_worldChange = false;

	_worldMatrix = glm::mat4(1.0);

	_worldMatrix[0][0] = _worldScale.x;
	_worldMatrix[1][1] = _worldScale.y;
	_worldMatrix[2][2] = _worldScale.z;

	_worldMatrix = glm::mat4_cast(_worldRotation) * _worldMatrix;

	_worldMatrix[3][0] = _worldPosition.x;
	_worldMatrix[3][1] = _worldPosition.y;
	_worldMatrix[3][2] = _worldPosition.z;
	_worldMatrix[3][3] = 1.0f;

	if (nullptr != _parent)
	{
		_localMatrix = glm::inverse(_parent->_worldMatrix) * _worldMatrix;
		glm::vec3 skew;
		glm::vec4 m;
		glm::decompose(_localMatrix, _localScale, _localRotation, _localPosition, skew, m);
		_localEulerAngles = glm::eulerAngles(_localRotation);
	}
	else
	{
		_localMatrix = _worldMatrix;
		_localPosition = _worldPosition;
		_localEulerAngles = _worldEulerAngles;
		_localRotation = _worldRotation;
		_localScale = _worldScale;
	}

	NotifyChange();
}

void Transform::NotifyChange()
{
	_entity->TransformChange();
}

void Transform::Tick()
{
	UpdateWhenLocalChange();
	UpdateWhenWorldChange();
}
