#include "CameraController.h"
#include "BehaviorImplement.inl"
#include "Entity.h"
#include "Event.h"
#include "EventCode.h"
#include "Camera.h"
#include "Transform.h"
#include "Renderer.h"
#include "EventSystemEditor.h"
#include "EventCodeEditor.h"
#include "EventEditor.h"
#include "EditViewPanelEditor.h"

using namespace LindaEditor;
using namespace LindaEngine;

bool CameraController::isLookRound = false;

IMPLEMENT_BEHAVIOR(CameraController)

void CameraController::Awake()
{
	_camera = _entity.GetComponent<PerspectiveCamera>();
	glm::vec3 front, up, right;
	_transform->GetWorldDir(front, up, right);
	_lookRoundDistance = 20.0f;
	_lookRoundTarget = _transform->GetWorldPosition() - front * _lookRoundDistance;

	Bind(EventCode::MouseWheel);
	Bind(EventCode::MouseWheelDown);
	Bind(EventCode::MouseWheelUp);
	Bind(EventCode::MouseMove);
	Bind(EventCode::LeftMouseButtonDown);
	Bind(EventCode::LeftMouseButtonUp);
	Bind(EventCode::RightMouseButtonDown);
	Bind(EventCode::RightMouseButtonUp);
	Bind(EventCode::MouseLeave);
	Bind(EventCode::KeyDown);
	Bind(EventCode::KeyUp);

	EventSystemEditor::Bind(EventCodeEditor::SwitchSelectEntity, this);
}

void CameraController::Update()
{

}

void CameraController::OnDestroy()
{
	Unbind(EventCode::MouseWheel);
	Unbind(EventCode::MouseWheelDown);
	Unbind(EventCode::MouseWheelUp);
	Unbind(EventCode::MouseMove);
	Unbind(EventCode::LeftMouseButtonDown);
	Unbind(EventCode::LeftMouseButtonUp);
	Unbind(EventCode::RightMouseButtonDown);
	Unbind(EventCode::RightMouseButtonUp);
	Unbind(EventCode::MouseLeave);
	Unbind(EventCode::KeyDown);
	Unbind(EventCode::KeyUp);

	EventSystemEditor::Unbind(EventCodeEditor::SwitchSelectEntity, this);
}

void CameraController::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	if (eventCode == EventCodeEditor::SwitchSelectEntity)
	{
		SwitchSelectEntityEditor& event = dynamic_cast<SwitchSelectEntityEditor&>(eventData);
		_selectedEntity = event.selectionEntity;
	}

	if (EditViewPanelEditor::hovered == false)
	{
		_isPanning = false;
		_leftHeld = false;
		_rightHeld = false;
		_ctrlHeld = false;
		isLookRound = false;
		return;
	}
	
	ProcessMouseEvent(eventCode, eventData);
	ProcessKeyEvent(eventCode, eventData);
}

void CameraController::ProcessMouseEvent(int eventCode, Event& eventData)
{
	if (eventCode == EventCode::KeyDown || eventCode == EventCode::KeyUp || eventCode == EventCodeEditor::SwitchSelectEntity)
		return;

	MouseEvent& event = dynamic_cast<MouseEvent&>(eventData);

	switch (eventCode)
	{
	case EventCode::MouseWheel:
		ScaleEvent(event);
		break;
	case EventCode::MouseWheelDown:
	{
		_isPanning = true;

		_lastMousePos.x = (float)event.x;
		_lastMousePos.y = (float)event.y;
	}
	break;
	case EventCode::MouseWheelUp:
	{
		_isPanning = false;
	}
	break;
	case EventCode::LeftMouseButtonDown:
	{
		_leftHeld = true;
		_lastMousePos.x = (float)event.x;
		_lastMousePos.y = (float)event.y;
	}
	break;
	case EventCode::LeftMouseButtonUp:
	{
		_leftHeld = false;
		isLookRound = false;
	}
	break;
	case EventCode::RightMouseButtonDown:
	{
		_rightHeld = true;
		_lastMousePos.x = (float)event.x;
		_lastMousePos.y = (float)event.y;
	}
	break;
	case EventCode::RightMouseButtonUp:
	{
		_rightHeld = false;
	}
	break;
	case EventCode::MouseMove:
		PanningEvent(event);
		LookRoundEvent(event);
		RotateEvent(event);
		break;
	case EventCode::MouseLeave:
	{
		_isPanning = false;
		_leftHeld = false;
		_rightHeld = false;
		_ctrlHeld = false;
		isLookRound = false;
	}
	break;
	}

}

void CameraController::ProcessKeyEvent(int eventCode, Event& eventData)
{
	if (eventCode != EventCode::KeyDown && eventCode != EventCode::KeyUp)
		return;

	KeyEvent& event = dynamic_cast<KeyEvent&>(eventData);

	switch (eventCode)
	{
	case EventCode::KeyDown:
	{
		std::cout << "	KeyDown  " << static_cast<int>(event.key) << "\n" << std::endl;
		if (event.key == KeyCode::CONTROL)
		{
			_ctrlHeld = true;
		}
		if (event.key == KeyCode::F)
		{
			LookAtEntity();
		}
	}
	break;
	case EventCode::KeyUp:
	{
		std::cout << "	KeyUp  " << static_cast<int>(event.key) << "\n" << std::endl;
		if (event.key == KeyCode::CONTROL)
		{
			_ctrlHeld = false;
			isLookRound = false;
		}

	}
	break;
	}

}

void CameraController::ScaleEvent(MouseEvent& event)
{
	glm::vec3 pos;
	pos.x = (float)event.x;
	pos.y = (float)event.y;
	pos.z = 1.0f;
	glm::vec3 far = _camera->ScreenToWorldPosition(pos);
	//glm::vec3 eyePos = _transform->GetWorldPosition();
	//glm::vec3 dir = glm::normalize(far - eyePos);
	//glm::vec3 worldPos = dir * glm::abs(far.y - eyePos.y);
	glm::vec3 moveVector = far * (event.wheel / _stanardWheelDelta) * _mouseWheelSpeed;
	_transform->SetWorldPosition(_transform->GetWorldPosition() + moveVector);

	_lookRoundTarget += moveVector;
}

void CameraController::PanningEvent(MouseEvent& event)
{
	if (false == _isPanning)
		return;
	glm::vec2 mouseDelta = _lastMousePos - glm::vec2(event.x, event.y);
	_lastMousePos.x = (float)event.x;
	_lastMousePos.y = (float)event.y;
	float distanceToTarget = glm::length(_transform->GetWorldPosition());//这里可以修改为相机到指向物体距离
	float adaptiveSpeed = _panSpeed * distanceToTarget;
	glm::vec3 front, up, right;
	_transform->GetWorldDir(front, up, right);
	glm::vec3 moveVector = (right * -mouseDelta.x + up * -mouseDelta.y) * adaptiveSpeed;
	_transform->SetWorldPosition(_transform->GetWorldPosition() + moveVector);

	_lookRoundTarget += moveVector;
}

void CameraController::RotateEvent(MouseEvent& event)
{
	if (false == _rightHeld)
		return;
	float xoffset = event.x - _lastMousePos.x;
	float yoffset = event.y - _lastMousePos.y;
	_lastMousePos.x = (float)event.x;
	_lastMousePos.y = (float)event.y;

	xoffset *= _rotateSpeed;
	yoffset *= _rotateSpeed;

	glm::vec3 angle = _transform->GetLocalEulerAngles();
	angle.x += yoffset;
	angle.y += xoffset;
	_transform->SetLocalEulerAngles(angle);

	glm::vec3 front, up, right;
	_transform->GetWorldDir(front, up, right);
	_lookRoundTarget = _transform->GetWorldPosition() - front * _lookRoundDistance;
}

void CameraController::LookRoundEvent(MouseEvent& event)
{
	if (false == _leftHeld || false == _ctrlHeld)
		return;
	isLookRound = true;

	float xoffset = event.x - _lastMousePos.x;
	float yoffset = event.y - _lastMousePos.y;
	_lastMousePos.x = (float)event.x;
	_lastMousePos.y = (float)event.y;

	xoffset *= _rotateSpeed;
	yoffset *= _rotateSpeed;

	glm::vec3 angle = _transform->GetLocalEulerAngles();
	angle.y += xoffset;
	angle.x += yoffset;
	_transform->SetLocalEulerAngles(angle);

	glm::vec3 front, up, right;
	_transform->GetLocalDir(front, up, right);
	_transform->SetWorldPosition(front * _lookRoundDistance + _lookRoundTarget);

}

void CameraController::LookAtEntity()
{
	if (_selectedEntity == nullptr)
		return;

	auto pair = CalculateLookAtData(_selectedEntity);

	glm::vec3 front, up, right;
	_transform->GetLocalDir(front, up, right);

	_lookRoundDistance = pair.first;
	_lookRoundTarget = pair.second;

	_transform->SetWorldPosition(pair.second + front * _lookRoundDistance);
}

std::pair<float, glm::vec3> CameraController::CalculateLookAtData(Entity* entity)
{
	float defaultDis = 6.6f;
	if (entity == nullptr)
		return { defaultDis, glm::vec3(0.0f)};
	
	Renderer* render = entity->GetComponent<Renderer>();
	if (render == nullptr)
		return { defaultDis, entity->GetTransform()->GetWorldPosition() };

	glm::vec3 target = render->GetBoundingBox().center;
	float aabbRadius = glm::length(render->GetBoundingBox().size * 0.5f);

	float fov = glm::radians(_camera->GetFOV());
	float verticalTan = glm::tan(fov * 0.5f);
	float distance = aabbRadius / verticalTan;
	if (_camera->GetAspectRatio() < 1)
	{
		//水平FOV = 2 × arctan( tan(垂直FOV / 2) × (宽高比) )
		fov = 2 * glm::atan(verticalTan * _camera->GetAspectRatio());
		distance = aabbRadius / glm::tan(fov * 0.5f);
	}
	float far = _camera->GetFar() * 0.6f;
	distance = distance > far ? far : distance;

	return { distance * 1.5f, target };
}

