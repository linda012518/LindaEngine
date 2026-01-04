#include "Camera.h"
#include "Entity.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "EventCode.h"
#include "Event.h"
#include "GraphicsContext.h"
#include "ComponentImplement.inl"
#include "CameraSystem.h"
#include "ClassFactory.h"
#include "TextureManager.h"

using namespace LindaEngine;

DYNAMIC_CREATE(PerspectiveCamera)
DYNAMIC_CREATE(OrthoCamera)
DYNAMIC_CREATE(CubeCamera)

Camera* Camera::currentRenderCamera = nullptr;

Camera::Camera(Entity& entity, bool enable) : Component(entity, enable)
{
	CameraSystem::Add(this);
	_transform->OnCameraAdded();

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
	_renderTextureDirty = false;

	_cameraType = CameraType::None;
	_clearType = CameraClearType::Skybox;
	_clearColor = glm::vec4(0.0);
	_depth = -1;
	_msaa = 1;
	_layerMask = 0;
	_renderTexture = nullptr;
}

Camera::~Camera()
{
	CameraSystem::Remove(this);
	_transform->OnCameraRemoved();
	_postStack.clear();
	if (nullptr != _renderTexture)
		RenderTextureManager::Release(_renderTexture);
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

	_frustum.UpdateFrustum(this);
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

void Camera::SetRenderTarget(Ref<RenderTexture> target)
{
	_renderTexture = target;
	_renderTextureDirty = true;
}

bool Camera::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::Key << "hdrEnable" << YAML::Value << _hdrEnable;
	out << YAML::Key << "zNear" << YAML::Value << _zNear;
	out << YAML::Key << "zFar" << YAML::Value << _zFar;
	out << YAML::Key << "depth" << YAML::Value << _depth;
	out << YAML::Key << "layerMask" << YAML::Value << _layerMask;
	out << YAML::Key << "msaa" << YAML::Value << _msaa;
	if (nullptr != _renderTexture)
		out << YAML::Key << "renderTexture" << YAML::Value << _renderTexture->nodePath;
	out << YAML::Key << "clearType" << YAML::Value << static_cast<int>(_clearType);
	out << YAML::Key << "clearColor" << YAML::Value << _clearColor;
	
	if (_postStack.size() > 0)
	{
		out << YAML::Key << "PostProcess";
		out << YAML::Value << YAML::BeginSeq;
		for (auto& pass : _postStack) {
			pass->Serialize();
		}
		out << YAML::EndSeq;
	}

	return true;
}

bool Camera::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	_hdrEnable = node["hdrEnable"].as<bool>();
	_zNear = node["zNear"].as<float>();
	_zFar = node["zFar"].as<float>();
	_depth = node["depth"].as<int>();
	_layerMask = node["layerMask"].as<int>();
	_msaa = node["msaa"].as<int>();
	if (node["renderTexture"])
		_renderTexture = YamlSerializer::DeSerializeRenderTexture(node["renderTexture"].as<std::string>().c_str());
	_clearType = static_cast<CameraClearType>(node["clearType"].as<int>());
	_clearColor = node["clearColor"].as<glm::vec4>();

	auto postStack = node["PostProcess"];
	if (!postStack)
		return true;

	for (auto pass : postStack)
	{
		std::string name = pass["Name"].as<std::string>();
		Ref<PostProcessEffectRenderer> renderer = ClassFactory<PostProcessEffectRenderer>::CreateObj(name);
		renderer->Deserialize(pass);
		_postStack.push_back(renderer);
	}

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
	Bind(EventCode::WindowResize);
}

PerspectiveCamera::~PerspectiveCamera()
{
	Unbind(EventCode::WindowResize);
}

void PerspectiveCamera::MakeProjectionMatrix()
{
	if (true == _renderTextureDirty)
	{
		_renderTextureDirty = false;
		_projectDirty = true;
		if (nullptr != _renderTexture)
			_aspectRatio = (float)_renderTexture->width / (float)_renderTexture->height;
		else
			_aspectRatio = (float)GraphicsContext::graphicsConfig.screenNewWidth / (float)GraphicsContext::graphicsConfig.screenNewHeight;
	}

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
	if (nullptr == _renderTexture && eventCode == EventCode::WindowResize)
	{
		WindowResizeEvent& wre = dynamic_cast<WindowResizeEvent&>(eventData);
		if (wre.isMinimized == true)
			return;
		_aspectRatio = (float)wre.width / (float)wre.height;
		_projectDirty = true;
	}
}

/////////////////////////////////////////////////////////////////////

OrthoCamera::OrthoCamera(Entity& entity, bool enable) : Camera(entity, enable)
{
	_cameraType = CameraType::OrthoCamera;
	_left = 100;
	_right = 100;
	_bottom = 100;
	_top = 100;
	Bind(EventCode::WindowResize);
}

OrthoCamera::~OrthoCamera()
{
	Unbind(EventCode::WindowResize);
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

CubeCamera::CubeCamera(Entity& entity, bool enable) : Camera(entity, enable)
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

	cameraMatrixs[0] = _transform->LookAt(glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	cameraMatrixs[1] = _transform->LookAt(glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	cameraMatrixs[2] = _transform->LookAt(glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
	cameraMatrixs[3] = _transform->LookAt(glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
	cameraMatrixs[4] = _transform->LookAt(glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	cameraMatrixs[5] = _transform->LookAt(glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
}

void CubeCamera::MakeProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, _zNear, _zFar);
}

void CubeCamera::MakeViewProjectionMatrix()
{
	if (false == _viewDirty && false == _projectDirty)
		return;
	_viewDirty = false;
	_projectDirty = false;

	cameraMatrixs[0] = projectionMatrix * cameraMatrixs[0];
	cameraMatrixs[1] = projectionMatrix * cameraMatrixs[1];
	cameraMatrixs[2] = projectionMatrix * cameraMatrixs[2];
	cameraMatrixs[3] = projectionMatrix * cameraMatrixs[3];
	cameraMatrixs[4] = projectionMatrix * cameraMatrixs[4];
	cameraMatrixs[5] = projectionMatrix * cameraMatrixs[5];
}

glm::mat4& CubeCamera::GetVPMatrix(int index)
{
	return cameraMatrixs[index];
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

