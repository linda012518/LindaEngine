#include "LObject.hpp"

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
