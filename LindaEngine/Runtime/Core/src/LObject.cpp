#include "LObject.h"

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
