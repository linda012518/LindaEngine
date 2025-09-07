#include "Component.hpp"

#include "Entity.hpp"

using namespace LindaEngine;

Component::Component()
{
	std::cout << "	Component" << std::endl;
}

Component::~Component()
{
	std::cout << "	~Component" << std::endl;
}

void Component::SetEntity(Entity* e)
{
	_entity = e;
}

void Component::Tick()
{
	std::cout << "Component  Tick Tick" << std::endl;
}
