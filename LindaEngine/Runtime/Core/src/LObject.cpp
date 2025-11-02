#include "LObject.h"
#include "EventSystem.h"

using namespace LindaEngine;

int LObject::_id = 0;
int LObject::_tempID = 0;

LObject::LObject()
{
	_selfID = _id;
	_id++;
	std::cout << "LObject" << _selfID << std::endl;
}

LObject::~LObject()
{
	_tempID++;
	std::cout << "~LObject" << _selfID << std::endl;
}

int LObject::GetID() const
{
	return _selfID;
}

void LObject::Bind(int eventCode)
{
	EventSystem::Bind(eventCode, this);
}

void LObject::Dispatch(LObject* sender, int eventCode, Event& eventData)
{
	EventSystem::Dispatch(sender, eventCode, eventData);
}

