#include "TestECSEvent.h"
#include "BehaviorImplement.inl"
#include "EventCode.h"
#include "Event.h"
#include "Input.h"

using namespace LindaEngine;

IMPLEMENT_BEHAVIOR(TestECSEvent)

void TestECSEvent::Awake()
{
	Bind(EventCode::WindowResize);
	Bind(EventCode::LeftMouseButtonDown);
	Bind(EventCode::LeftMouseButtonUp);
	Bind(EventCode::LeftMouseButton);
	Bind(EventCode::RightMouseButtonDown);
	Bind(EventCode::RightMouseButtonUp);
	Bind(EventCode::RightMouseButton);
	Bind(EventCode::MouseMove);
	Bind(EventCode::MouseWheel);
	Bind(EventCode::KeyDown);
	Bind(EventCode::KeyUp);
	Bind(EventCode::Key);
	Bind(EventCode::CharInput);
	Bind(EventCode::KeyClick);
	Bind(EventCode::LeftMouseButtonClick);
	Bind(EventCode::RightMouseButtonClick);
	Bind(EventCode::MouseWheelClick);
	Bind(EventCode::MouseWheelDown);
	Bind(EventCode::MouseWheelUp);
	Bind(EventCode::MouseWheelPressed);
}

void TestECSEvent::Update()
{
	if (Input::GetMouseButtonDown(MouseCode::ButtonLeft))
	{
		std::cout << "	GetMouseButtonDown  0 \n" << std::endl;
	}
	if (Input::GetMouseButtonDown(MouseCode::Button1))
	{
		std::cout << "	GetMouseButtonDown  1 \n" << std::endl;
	}
	if (Input::GetMouseButtonUp(MouseCode::Button0))
	{
		std::cout << "	GetMouseButtonUp  0 \n" << std::endl;
	}
	if (Input::GetMouseButtonUp(MouseCode::ButtonRight))
	{
		std::cout << "	GetMouseButtonUp  1 \n" << std::endl;
	}
	if (Input::GetMouseButton(MouseCode::Button0))
	{
		std::cout << "	GetMouseButton  0 \n" << std::endl;
	}
	if (Input::GetMouseButton(MouseCode::ButtonRight))
	{
		std::cout << "	GetMouseButton  1 \n" << std::endl;
	}
	if (Input::GetKeyDown(KeyCode::D))
	{
		std::cout << "	GetKeyDown  " << static_cast<int>(KeyCode::D) << "   \n" << std::endl;
	}
	if (Input::GetKey(KeyCode::E))
	{
		std::cout << "	GetKey  " << static_cast<int>(KeyCode::E) << "   \n" << std::endl;
	}
	if (Input::GetKeyUp(KeyCode::F))
	{
		std::cout << "	GetKeyUp  " << static_cast<int>(KeyCode::F) << "   \n" << std::endl;
	}
	{
		std::cout << "	GetMousePosition  " << Input::GetMousePosition().x << "  " << Input::GetMousePosition().y << std::endl;
	}
}

void TestECSEvent::OnEvent(IEventHandler* sender, int eventCode, Event& eventData)
{
	if (eventCode == EventCode::LeftMouseButtonDown)
	{
		std::cout << "	LeftMouseButtonDown \n" << std::endl;
	}
	else if (eventCode == EventCode::LeftMouseButtonUp)
	{
		std::cout << "	LeftMouseButtonUp \n" << std::endl;
	}
	else if (eventCode == EventCode::LeftMouseButton)
	{
		std::cout << "	LeftMouseButton \n" << std::endl;
	}
	else if (eventCode == EventCode::RightMouseButtonDown)
	{
		std::cout << "	RightMouseButtonDown \n" << std::endl;
	}
	else if (eventCode == EventCode::RightMouseButtonUp)
	{
		std::cout << "	RightMouseButtonUp \n" << std::endl;
	}
	else if (eventCode == EventCode::RightMouseButton)
	{
		std::cout << "	RightMouseButton \n" << std::endl;
	}
	else if (eventCode == EventCode::MouseMove)
	{
		MouseEvent& me = dynamic_cast<MouseEvent&>(eventData);
		//std::cout << "	MouseMove  " << me.x << "   " << me.y << "\n" << std::endl;
	}
	else if (eventCode == EventCode::MouseWheel)
	{
		MouseEvent& me = dynamic_cast<MouseEvent&>(eventData);
		std::cout << "	MouseWheel  " << me.wheel << "\n" << std::endl;
	}
	else if (eventCode == EventCode::KeyDown)
	{
		KeyEvent& me = dynamic_cast<KeyEvent&>(eventData);
		std::cout << "	KeyDown  " << static_cast<int>(me.key) << "\n" << std::endl;
	}
	else if (eventCode == EventCode::KeyUp)
	{
		KeyEvent& me = dynamic_cast<KeyEvent&>(eventData);
		std::cout << "	KeyUp  " << static_cast<int>(me.key) << "\n" << std::endl;
	}
	else if (eventCode == EventCode::Key)
	{
		KeyEvent& me = dynamic_cast<KeyEvent&>(eventData);
		std::cout << "	Key---  " << static_cast<int>(me.key) << "\n" << std::endl;
	}
	else if (eventCode == EventCode::CharInput)
	{
		CharEvent& me = dynamic_cast<CharEvent&>(eventData);
		std::cout << "	CharInput  " << me.key << "   " << "\n" << std::endl;
	}
	else if (eventCode == EventCode::LeftMouseButtonClick)
	{
		std::cout << "	LeftMouseButtonClick \n" << std::endl;
	}
	else if (eventCode == EventCode::RightMouseButtonClick)
	{
		std::cout << "	RightMouseButtonClick \n" << std::endl;
	}
	else if (eventCode == EventCode::KeyClick)
	{
		KeyEvent& me = dynamic_cast<KeyEvent&>(eventData);
		std::cout << "	KeyClick  " << static_cast<int>(me.key) << "\n" << std::endl;
	}
	else if (eventCode == EventCode::WindowResize)
	{
		WindowResizeEvent& wre = dynamic_cast<WindowResizeEvent&>(eventData);
		std::cout << "	WindowResize  " << wre.width << "   " << wre.height << "\n" << std::endl;
	}
	else if (eventCode == EventCode::MouseWheelClick)
	{
		MouseEvent& me = dynamic_cast<MouseEvent&>(eventData);
		std::cout << "	MouseWheelClick  \n" << std::endl;
	}
	else if (eventCode == EventCode::MouseWheelDown)
	{
		MouseEvent& me = dynamic_cast<MouseEvent&>(eventData);
		std::cout << "	MouseWheelDown  \n" << std::endl;
	}
	else if (eventCode == EventCode::MouseWheelUp)
	{
		MouseEvent& me = dynamic_cast<MouseEvent&>(eventData);
		std::cout << "	MouseWheelUp  \n" << std::endl;
	}
	else if (eventCode == EventCode::MouseWheelPressed)
	{
		MouseEvent& me = dynamic_cast<MouseEvent&>(eventData);
		std::cout << "	MouseWheelPressed  \n" << std::endl;
	}
}

