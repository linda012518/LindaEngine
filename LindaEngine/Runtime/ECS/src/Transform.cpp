#include "Transform.h"
#include "Entity.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "Scene.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/euler_angles.hpp"

#define PI 3.14159265358979323f

#define DO_LOCALCHANGE \
if (_localChange) \
	UpdateWhenLocalChange(); \
_worldChange = true; 

#define DO_WORLDCHANGE \
if (_worldChange) \
	UpdateWhenWorldChange(); \
_localChange = true;

using namespace LindaEngine;

Transform::Transform(Entity& entity) : Component(entity, true)
{
	std::cout << "	Transform" << _selfID << std::endl;
}

Transform::~Transform()
{
	std::cout << "	~Transform" << _selfID << std::endl;
}

const glm::mat4& Transform::GetLocalToWorldMat() const
{
	return _worldMatrix;
}

const glm::mat4& Transform::GetWorldToLocalMat() const
{
	return _worldInverseMatrix;
}

const glm::mat4& Transform::GetViewMat() const
{
	return _viewMatrix;
}

const Transform* Transform::GetParent() const
{
	return _parent;
}

const std::list<Transform*>& Transform::GetChildren() const
{
	return _children;
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

const glm::vec3& Transform::GetWorldScale() const
{
	return _worldScale;
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

void Transform::SetParent(Transform* parent)
{
	if (_parent == parent)
		return;

	if (_localChange)
		UpdateWhenLocalChange();
	_worldChange = true;

	if (_parent)
		_parent->_children.remove(this);

	_parent = parent;

	if (nullptr != _parent)
	{
		_parent->_children.push_back(this);
		_parentID = _parent->GetEntity().GetUUID();
	}
	else
	{
		_parentID = "";
	}
}

void Transform::GetWorldDir(glm::vec3& forward, glm::vec3& up, glm::vec3& right)
{
	GetDir(_worldRotation, forward, up, right);
}

void Transform::GetLocalDir(glm::vec3& forward, glm::vec3& up, glm::vec3& right)
{
	GetDir(_localRotation, forward, up, right);
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
	_localRotation = glm::quat(glm::radians(angle));
}

void Transform::SetLocalRotation(const glm::quat& rotation)
{
	if (_worldChange)
		UpdateWhenWorldChange();
	_localChange = true;

	_localRotation = rotation;
	_localEulerAngles = glm::eulerAngles(rotation) * 180.f / PI;
}

void Transform::SetLocalScale(const glm::vec3& scale)
{
	if (_worldChange)
		UpdateWhenWorldChange();
	_localChange = true;

	_localScale = scale;
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
	_worldRotation = glm::quat(glm::radians(angle));
}

void Transform::SetWorldRotation(const glm::quat& rotation)
{
	if (_localChange)
		UpdateWhenLocalChange();
	_worldChange = true;

	_worldRotation = rotation;
	_worldEulerAngles = glm::eulerAngles(rotation) * 180.f / PI;
}

void Transform::SetWorldScale(const glm::vec3& scale)
{
	if (_localChange)
		UpdateWhenLocalChange();
	_worldChange = true;

	_worldScale = scale;
}

const glm::mat4& Transform::LookAt(const glm::vec3& center, const glm::vec3& up)
{
	return glm::lookAt(_worldPosition, _worldPosition + center, up);
}

void Transform::CalculateWordMatrix()
{
	UpdateWhenWorldChange();
}

void Transform::Tick()
{
	if (_entity.IsActive() == false)
		return;

	UpdateWhenLocalChange();
	UpdateWhenWorldChange();
}

void Transform::Destroy()
{
	_parent = nullptr;
	_children.clear();
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
		_worldEulerAngles = glm::eulerAngles(_worldRotation) * 180.f / PI;
	}
	else
	{
		_worldMatrix = _localMatrix;
		_worldEulerAngles = _localEulerAngles;
		_worldPosition = _localPosition;
		_worldScale = _localScale;
		_worldRotation = _localRotation;
	}

	_worldInverseMatrix = glm::inverse(_worldMatrix);

	UpdateViewMatrix();
	NotifyChange();
	UpdateChildren(_children);
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

	_worldInverseMatrix = glm::inverse(_worldMatrix);

	if (nullptr != _parent)
	{
		_localMatrix = glm::inverse(_parent->_localMatrix) * _worldMatrix;
		glm::vec3 skew;
		glm::vec4 m;
		glm::decompose(_localMatrix, _localScale, _localRotation, _localPosition, skew, m);
		_localEulerAngles = glm::eulerAngles(_localRotation) * 180.f / PI;
	}
	else
	{
		_localMatrix = _worldMatrix;
		_localPosition = _worldPosition;
		_localEulerAngles = _worldEulerAngles;
		_localRotation = _worldRotation;
		_localScale = _worldScale;
	}

	UpdateViewMatrix();
	NotifyChange();
	UpdateChildren(_children);

}

void Transform::UpdateViewMatrix()
{
	if (false == _hasCamera)
		return;

	_viewMatrix = glm::mat4_cast(_worldRotation);

	_viewMatrix[3][0] = _worldPosition.x;
	_viewMatrix[3][1] = _worldPosition.y;
	_viewMatrix[3][2] = _worldPosition.z;
	_viewMatrix[3][3] = 1.0f;

	_viewMatrix = glm::inverse(_viewMatrix);

}

void Transform::UpdateChildren(std::list<Transform*> children)
{
	if (children.size() <= 0)
		return;

	glm::mat4& worldMat = _worldMatrix;
	glm::vec3 skew;
	glm::vec4 m;

	for (auto& com : children) {
		com->_worldMatrix = worldMat * com->_localMatrix;
		glm::decompose(com->_worldMatrix, com->_worldScale, com->_worldRotation, com->_worldPosition, skew, m);
		com->_worldEulerAngles = glm::eulerAngles(com->_worldRotation);
		com->_worldInverseMatrix = glm::inverse(com->_worldMatrix);

		UpdateViewMatrix();
		NotifyChange();
		UpdateChildren(com->_children);
	}

}

void Transform::NotifyChange()
{
	//通知其它系统变更
	_entity.TransformDirty();
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

bool Transform::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "Name" << YAML::Value << "Transform";
	//out << YAML::Key << "localPosition" << YAML::Value << _localPosition;
	//out << YAML::Key << "localEulerAngles" << YAML::Value << _localEulerAngles;
	//out << YAML::Key << "localScale" << YAML::Value << _localScale;
	//out << YAML::Key << "localRotation" << YAML::Value << _localRotation;

	out << YAML::Key << "worldPosition" << YAML::Value << _worldPosition;
	//out << YAML::Key << "worldEulerAngles" << YAML::Value << _worldEulerAngles;
	out << YAML::Key << "worldScale" << YAML::Value << _worldScale;
	out << YAML::Key << "worldRotation" << YAML::Value << _worldRotation;

	out << YAML::Key << "parent" << YAML::Value << _parentID;

	out << YAML::EndMap;

	return true;
}

bool Transform::Deserialize(YAML::Node& node)
{
	glm::vec3 pos = node["worldPosition"].as<glm::vec3>();
	glm::vec3 scale = node["worldScale"].as<glm::vec3>();
	glm::quat rotation = node["worldRotation"].as<glm::quat>();
	std::string parent = node["parent"].as<std::string>();
	SetWorldPosition(pos);
	SetWorldScale(scale);
	SetWorldRotation(rotation);
	SetParentID(parent);
	return true;
}

