#include "LObject.h"
#include "EventSystem.h"

using namespace LindaEngine;

int LObject::_id = 0;
int LObject::_tempID = 0;

LObject::LObject()
{
	_selfID = _id;
	_id++;
}

LObject::~LObject()
{
	_tempID++;
}

int LObject::GetID() const
{
	return _selfID;
}

void LObject::Bind(int eventCode)
{
	EventSystem::Bind(eventCode, this);
}

void LObject::Dispatch(IEventHandler* sender, int eventCode, Event& eventData)
{
	EventSystem::Dispatch(sender, eventCode, eventData);
}

