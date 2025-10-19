#include "Camera.h"
#include "Entity.h"
#include "Transform.h"
#include "YamlSerializer.h"

using namespace LindaEngine;

DYNAMIC_CREATE(PerspectiveCamera)
DYNAMIC_CREATE(OrthoCamera)
DYNAMIC_CREATE(CubeCamera)

Camera::Camera(Entity& entity, bool enable) : Component(entity, enable)
{
	_viewMatrix = glm::mat4(1.0);
	_projectMatrix = glm::mat4(1.0);
	_viewProjectMatrix = glm::mat4(1.0);

	_viewInverseMatrix = glm::mat4(1.0);
	_projectInverseMatrix = glm::mat4(1.0);
	_viewProjectInverseMatrix = glm::mat4(1.0);

	_zNear = 0.1f;
	_zFar = 1000.0f;

	_viewDirty = true;
	_projectDirty = true;

	_cameraType = CameraType::None;
	_clearType = CameraClearType::Skybox;
	_depth = -1;
}

Camera::~Camera()
{
}

void Camera::MakeViewMatrix()
{
	if (false == _viewDirty)
		return;

	Transform* t = _entity.GetComponent<Transform>();
	if (nullptr != t)
	{
		_viewMatrix = _viewMatrix;
		_viewInverseMatrix = glm::inverse(_viewMatrix);
	}
}

void Camera::MakeViewProjectionMatrix()
{
	if (false == _viewDirty && false == _projectDirty)
		return;
	_viewDirty = false;
	_projectDirty = false;

	_viewProjectMatrix = _projectMatrix * _viewMatrix;
	_viewProjectInverseMatrix = glm::inverse(_viewProjectMatrix);
}

void Camera::Tick()
{
	if (_enable == false || _entity.IsActive() == false)
		return;

	MakeViewMatrix();
	MakeProjectionMatrix();
	MakeViewProjectionMatrix();
}

void Camera::TransformChange()
{
	_viewDirty = true;
}

void Camera::SetNearFar(float near, float far, float dontCare)
{
	_projectDirty = true;
	if (near != dontCare)
		_zNear = near;
	if (far != dontCare)
		_zFar = far;
}

/////////////////////////////////////////////////////////////////////

PerspectiveCamera::PerspectiveCamera(Entity& entity, bool enable) : Camera(entity, enable)
{
	_cameraType = CameraType::PerspectiveCamera;
	_fov = 60.0f;
	_aspectRatio = 1.777778f;
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::MakeProjectionMatrix()
{
	_projectMatrix = glm::perspective(glm::radians(_fov), _aspectRatio, _zNear, _zFar);
	_projectInverseMatrix = glm::inverse(_projectMatrix);
}

void PerspectiveCamera::SetProjectionData(float fov, float ratio, float near, float far, float dontCare)
{
	if (fov != dontCare)
		_fov = fov;
	if (ratio != dontCare)
		_aspectRatio = ratio;

	SetNearFar(near, far, dontCare);
}

bool PerspectiveCamera::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "Name" << YAML::Value << "PerspectiveCamera";
	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::Key << "fov" << YAML::Value << _fov;
	out << YAML::Key << "zNear" << YAML::Value << _zNear;
	out << YAML::Key << "zFar" << YAML::Value << _zFar;
	out << YAML::Key << "depth" << YAML::Value << _depth;
	out << YAML::Key << "cameraType" << YAML::Value << static_cast<int>(_cameraType);
	out << YAML::Key << "clearType" << YAML::Value << static_cast<int>(_clearType);

	out << YAML::EndMap;

	return true;
}

bool PerspectiveCamera::Deserialize(YAML::Node& node)
{
	int a = 0;
	return true;
}

/////////////////////////////////////////////////////////////////////

OrthoCamera::OrthoCamera(Entity& entity, bool enable) : Camera(entity, enable)
{
	_cameraType = CameraType::OrthoCamera;
	_left = 100;
	_right = 100;
	_bottom = 100;
	_top = 100;
}

OrthoCamera::~OrthoCamera()
{

}

void OrthoCamera::MakeProjectionMatrix()
{
	_projectMatrix = glm::ortho(_left, _right, _bottom, _top, _zNear, _zFar);
	_projectInverseMatrix = glm::inverse(_projectMatrix);
}

void OrthoCamera::SetProjectionData(float left, float right, float top, float bottom, float near, float far, float dontCare)
{
	if (left != dontCare)
		_left = left;
	if (right != dontCare)
		_right = right;
	if (top != dontCare)
		_top = top;
	if (bottom != dontCare)
		_bottom = bottom;

	SetNearFar(near, far, dontCare);
}

bool OrthoCamera::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	//out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
	//out << YAML::Key << "Value" << YAML::Value << pointer->value;
	//out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
	out << YAML::EndMap;

	return true;
}

bool OrthoCamera::Deserialize(YAML::Node& node)
{
	return true;
}

/////////////////////////////////////////////////////////////////////

CubeCamera::CubeCamera(Entity& entity, bool enable) : Camera(entity, enable),
	_right(entity, enable), _left(entity, enable), _top(entity, enable), _bottom(entity, enable), _back(entity, enable), _front(entity, enable)
{
	_cameraType = CameraType::CubeCamera;
}

CubeCamera::~CubeCamera()
{

}

void CubeCamera::MakeViewMatrix()
{
	if (false == _viewDirty)
		return;

	Transform* t = _entity.GetComponent<Transform>();

	_right._viewMatrix	= t->LookAt(glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_left._viewMatrix	= t->LookAt(glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_top._viewMatrix	= t->LookAt(glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
	_bottom._viewMatrix = t->LookAt(glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
	_back._viewMatrix	= t->LookAt(glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_front._viewMatrix	= t->LookAt(glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));

}

void CubeCamera::MakeProjectionMatrix()
{
}

void CubeCamera::MakeViewProjectionMatrix()
{
	if (false == _viewDirty && false == _projectDirty)
		return;
	_viewDirty = false;
	_projectDirty = false;


	glm::mat4 pro = glm::perspective(glm::radians(90.0f), 1.0f, _zNear, _zFar);
	_right._viewProjectMatrix = pro * _right._viewMatrix;
	_left._viewProjectMatrix = pro * _left._viewMatrix;
	_top._viewProjectMatrix = pro * _top._viewMatrix;
	_bottom._viewProjectMatrix = pro * _bottom._viewMatrix;
	_back._viewProjectMatrix = pro * _back._viewMatrix;
	_front._viewProjectMatrix = pro * _front._viewMatrix;
}

void CubeCamera::SetProjectionData(float near, float far, float dontCare)
{
	SetNearFar(near, far, dontCare);
}

bool CubeCamera::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;
	//out << YAML::Key << "UniformName" << YAML::Value << pointer->name;
	//out << YAML::Key << "Value" << YAML::Value << pointer->value;
	//out << YAML::Key << pointer->name << YAML::Value << static_cast<int>(pointer->dataType);
	out << YAML::EndMap;

	return true;
}

bool CubeCamera::Deserialize(YAML::Node& node)
{
	return true;
}

