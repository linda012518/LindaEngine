#include "CameraController.h"
#include "BehaviorImplement.inl"
#include "Entity.h"
#include "Event.h"
#include "EventCode.h"
#include "Camera.h"

using namespace LindaEngine;

IMPLEMENT_BEHAVIOR(CameraController)

void CameraController::Awake()
{
	_camera = _entity.GetComponent<Camera>();
	Bind(EventCode::MouseWheel);
	Bind(EventCode::MouseWheelPressed);
}

void CameraController::Update()
{

}

void CameraController::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	MouseEvent& event = dynamic_cast<MouseEvent&>(eventData);
	std::cout << "	MouseWheel  " << event.wheel << "\n" << std::endl;
	switch (eventCode)
	{
	case EventCode::MouseWheel:
		break;
	case EventCode::MouseWheelPressed:
		break;
	}
}

