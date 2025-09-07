#include "LObject.hpp"
#include "Common/EventSystem.h"

using namespace LindaEngine;

int LObject::_id = 0;

LObject::LObject()
{
	_selfID = _id;
	_id++;
	std::cout << "LObject" << std::endl;
}

LObject::~LObject()
{
	std::cout << "~LObject" << std::endl;
}

int LObject::GetID() const
{
	return _selfID;
}

void LObject::Bind(int eventCode)
{
	EventSystem::Bind(eventCode, this);
}

void LObject::Dispatch(int eventCode, void* userData)
{
	EventSystem::Dispatch(eventCode, userData);
}

void LObject::OnEvent(void* userData)
{
	std::cout << "LObject::OnEvent" << std::endl;
}
