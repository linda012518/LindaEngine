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
#include "GUILayoutEditor.h"

#include "imgui/imgui.h"

using namespace LindaEngine;
using namespace LindaEditor;

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
	_clearColor = glm::vec4(0.0, 0.0, 0.0, 1.0);
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

void Camera::AddPostProcess(Ref<PostProcessEffectRenderer> postProcess)
{
	if (nullptr != postProcess)
		_postStack.push_back(postProcess);
}

Ref<PostProcessEffectRenderer> Camera::AddPostProcess(std::string postProcess)
{
	Ref<PostProcessEffectRenderer> renderer = ClassFactory<PostProcessEffectRenderer>::CreateObj(postProcess);
	AddPostProcess(renderer);
	return renderer;
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
		Ref<PostProcessEffectRenderer> renderer = AddPostProcess(name);
		renderer->Deserialize(pass);
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

CameraClearType Camera::GetClearTypeByString(std::string str)
{
	if (str == "Skybox") return CameraClearType::Skybox;
	else if (str == "SolidColor") return CameraClearType::SolidColor;
	else if (str == "DepthOnly") return CameraClearType::DepthOnly;
	else if (str == "DontClear") return CameraClearType::DontClear;
	return CameraClearType::Skybox;
}

std::string Camera::GetStringByClearType(CameraClearType type)
{
	if (type == CameraClearType::Skybox) return "Skybox";
	else if (type == CameraClearType::SolidColor) return "SolidColor";
	else if (type == CameraClearType::DepthOnly) return "DepthOnly";
	else if (type == CameraClearType::DontClear) return "DontClear";
	return "Skybox";
}

void Camera::OnImguiRender()
{
	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "ClearType", "ClearColor", "Depth", "MSAA" });

	GUILayoutEditor::DragFloat("Near", &_zNear, [&]() {
		if (_zNear < 0.01f)
			_zNear = 0.01f;
		else if (_zNear > _zFar - 0.1f)
			_zNear = _zFar - 0.1f;
		SetNearFar(_zNear, -1.0f);
		}, 0.01f, 0.01f, _zFar - 0.1f, firstWidth);

	GUILayoutEditor::DragFloat("Far", &_zFar, [&]() {
		if (_zFar < _zNear + 0.1f)
			_zFar = _zNear + 0.1f;
		else if (_zFar > 10000.0f)
			_zFar = 10000.0f;
		SetNearFar(-1.0f, _zFar);
		}, 0.01f, _zNear + 0.1f, 10000.0f, firstWidth);

	GUILayoutEditor::DragInt("Depth", &_depth, nullptr, 1, 0, 0, firstWidth);

	GUILayoutEditor::DragInt("MSAA", &_msaa, [&]() {
		//TODO 这里要清掉有前的RT
		}, 1, 1, 8, firstWidth);

	GUILayoutEditor::ColorEdit4("ClearColor", (float*)&_clearColor, nullptr, firstWidth);

	GUILayoutEditor::Checkbox("HDR", &_hdrEnable, [&]() {
		//TODO 这里要清掉有前的RT
		}, firstWidth);

	static std::vector<std::string> names = { "Skybox", "SolidColor", "DepthOnly", "DontClear" };
	GUILayoutEditor::Dropdown("ClearType", (int)_clearType, names, [&](int index) {
		_clearType = GetClearTypeByString(names[index]);
		});
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

void PerspectiveCamera::OnImguiRender()
{
	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "ClearType", "ClearColor", "Depth", "MSAA" });

	GUILayoutEditor::DragFloat("FOV", &_fov, [&]() {
			if (_fov < 0.1f)
				_fov = 0.1f;
			else if (_fov > 179.0f)
				_fov = 179.0f;
			SetProjectionData(_fov, -1.0f, -1.0f, -1.0f);
		}, 0.5f, 0.1f, 179.0f, firstWidth);

	Camera::OnImguiRender();
}

/////////////////////////////////////////////////////////////////////

OrthoCamera::OrthoCamera(Entity& entity, bool enable) : Camera(entity, enable)
{
	_cameraType = CameraType::OrthoCamera;
	_halfWidth = 100.0f;
	_halfHeight = 100.0f;
	_scale = 1.0f;
	_aspectRatio = (float)GraphicsContext::graphicsConfig.screenNewWidth / (float)GraphicsContext::graphicsConfig.screenNewHeight;
	_projectSkyboxMatrix = glm::mat4(1.0f);
	Bind(EventCode::WindowResize);
}

OrthoCamera::~OrthoCamera()
{
	Unbind(EventCode::WindowResize);
}

const glm::mat4& OrthoCamera::GetSkyboxProjectMatrix()
{
	return _projectSkyboxMatrix;
}

void OrthoCamera::MakeProjectionMatrix()
{
	if (false == _projectDirty)
		return;

	_projectMatrix = glm::ortho(-_halfWidth * _scale, _halfWidth * _scale, -_halfHeight * _scale, _halfHeight * _scale, _zNear, _zFar);
	_projectInverseMatrix = glm::inverse(_projectMatrix);

	_projectSkyboxMatrix = glm::perspective(glm::radians(60.0f), _aspectRatio, _zNear, _zFar);
}

bool OrthoCamera::Serialize()
{
	Camera::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Key << "Name" << YAML::Value << "OrthoCamera";
	out << YAML::Key << "halfWidth" << YAML::Value << _halfWidth;
	out << YAML::Key << "halfHeight" << YAML::Value << _halfHeight;
	out << YAML::Key << "scale" << YAML::Value << _scale;
	
	out << YAML::EndMap;

	return true;
}

bool OrthoCamera::Deserialize(YAML::Node& node)
{
	Camera::Deserialize(node);
	_halfWidth = node["halfWidth"].as<float>();
	_halfHeight = node["halfHeight"].as<float>();
	_scale = node["scale"].as<float>();
	return true;
}

void OrthoCamera::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	if (nullptr == _renderTexture && eventCode == EventCode::WindowResize)
	{
		WindowResizeEvent& wre = dynamic_cast<WindowResizeEvent&>(eventData);
		if (wre.isMinimized == true)
			return;
		_aspectRatio = (float)wre.width / (float)wre.height;
		_halfWidth = wre.width * 0.5f;
		_halfHeight = wre.height * 0.5f;
		_projectDirty = true;
	}
}

void OrthoCamera::OnImguiRender()
{
	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "ClearType", "ClearColor", "Depth", "MSAA" });

	GUILayoutEditor::DragFloat("Size", &_scale, [&]() {
		if (_scale < 0.1f)
			_scale = 0.1f;
		else if (_scale > 100.0f)
			_scale = 100.0f;
		_projectDirty = true;
		}, 0.1f, 0.1f, 100.0f, firstWidth);

	Camera::OnImguiRender();
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

	viewMatrixs[0] = _transform->LookAt(glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	viewMatrixs[1] = _transform->LookAt(glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	viewMatrixs[2] = _transform->LookAt(glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
	viewMatrixs[3] = _transform->LookAt(glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
	viewMatrixs[4] = _transform->LookAt(glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	viewMatrixs[5] = _transform->LookAt(glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
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

	viewProjectionMatrixs[0] = projectionMatrix * viewMatrixs[0];
	viewProjectionMatrixs[1] = projectionMatrix * viewMatrixs[1];
	viewProjectionMatrixs[2] = projectionMatrix * viewMatrixs[2];
	viewProjectionMatrixs[3] = projectionMatrix * viewMatrixs[3];
	viewProjectionMatrixs[4] = projectionMatrix * viewMatrixs[4];
	viewProjectionMatrixs[5] = projectionMatrix * viewMatrixs[5];
}

glm::mat4& CubeCamera::GetCubeViewMatrix(int index)
{
	return viewMatrixs[index];
}

glm::mat4& CubeCamera::GetCubeProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4& CubeCamera::GetCubeVPMatrix(int index)
{
	return viewProjectionMatrixs[index];
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

