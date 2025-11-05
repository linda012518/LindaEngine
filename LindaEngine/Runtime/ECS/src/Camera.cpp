#include "Camera.h"
#include "Entity.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "EventCode.h"
#include "Event.h"
#include "GraphicsContext.h"
#include "ComponentImplement.inl"

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

	Bind(EventCode::WindowResize);
}

Camera::~Camera()
{
}

void Camera::MakeViewMatrix()
{
	if (false == _viewDirty)
		return;

	if (nullptr != _transform)
	{
		_viewMatrix = _transform->GetViewMat();
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

void Camera::TransformDirty()
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

bool Camera::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::Key << "zNear" << YAML::Value << _zNear;
	out << YAML::Key << "zFar" << YAML::Value << _zFar;
	out << YAML::Key << "depth" << YAML::Value << _depth;
	out << YAML::Key << "clearType" << YAML::Value << static_cast<int>(_clearType);

	return true;
}

bool Camera::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	_zNear = node["zNear"].as<float>();
	_zFar = node["zFar"].as<float>();
	_depth = node["depth"].as<int>();
	_clearType = static_cast<CameraClearType>(node["clearType"].as<int>());
	return true;
}

glm::vec3 Camera::ScreenToWorldPosition(glm::vec3& screenPos)
{
	glm::vec4 v;
	v.x = screenPos.x;
	v.y = screenPos.y;
	v.z = screenPos.z;
	v.w = 1.0;

	v.x = (v.x) / GraphicsContext::graphicsConfig.screenNewWidth;
	v.y = (GraphicsContext::graphicsConfig.screenNewHeight - v.y) / GraphicsContext::graphicsConfig.screenNewHeight;

	v.x = v.x * 2.0f - 1.0f;
	v.y = v.y * 2.0f - 1.0f;
	v.z = v.z * 2.0f - 1.0f;

	v = _viewProjectInverseMatrix * v;
	v = v / v.w;

	return v;
}

/////////////////////////////////////////////////////////////////////

PerspectiveCamera::PerspectiveCamera(Entity& entity, bool enable) : Camera(entity, enable)
{
	_cameraType = CameraType::PerspectiveCamera;
	_fov = 60.0f;
	_aspectRatio = (float)GraphicsContext::graphicsConfig.screenNewWidth / (float)GraphicsContext::graphicsConfig.screenNewHeight;
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::MakeProjectionMatrix()
{
	if (false == _projectDirty)
		return;

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
	Camera::Serialize();
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Key << "Name" << YAML::Value << "PerspectiveCamera";
	out << YAML::Key << "fov" << YAML::Value << _fov;

	out << YAML::EndMap;

	return true;
}

bool PerspectiveCamera::Deserialize(YAML::Node& node)
{
	Camera::Deserialize(node);
	_fov = node["fov"].as<float>();
	return true;
}

void PerspectiveCamera::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	WindowResizeEvent& wre = dynamic_cast<WindowResizeEvent&>(eventData);
	_aspectRatio = (float)wre.width / (float)wre.height;
	_projectDirty = true;
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
	if (false == _projectDirty)
		return;

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
	Camera::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Key << "Name" << YAML::Value << "OrthoCamera";
	out << YAML::Key << "left" << YAML::Value << _left;
	out << YAML::Key << "right" << YAML::Value << _right;
	out << YAML::Key << "bottom" << YAML::Value << _bottom;
	out << YAML::Key << "top" << YAML::Value << _top;

	out << YAML::EndMap;

	return true;
}

bool OrthoCamera::Deserialize(YAML::Node& node)
{
	Camera::Deserialize(node);
	_left = node["left"].as<float>();
	_right = node["right"].as<float>();
	_bottom = node["bottom"].as<float>();
	_top = node["top"].as<float>();
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

	_right._viewMatrix	= _transform->LookAt(glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_left._viewMatrix	= _transform->LookAt(glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_top._viewMatrix	= _transform->LookAt(glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
	_bottom._viewMatrix = _transform->LookAt(glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
	_back._viewMatrix	= _transform->LookAt(glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_front._viewMatrix	= _transform->LookAt(glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));

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
	Camera::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Key << "Name" << YAML::Value << "CubeCamera";

	out << YAML::EndMap;

	return true;
}

bool CubeCamera::Deserialize(YAML::Node& node)
{
	return Camera::Deserialize(node);
}

