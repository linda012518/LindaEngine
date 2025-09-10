#include "Component.hpp"

#include "Entity.hpp"

using namespace LindaEngine;

Component::Component()
{
	std::cout << "	Component" << _selfID << std::endl;
}

Component::~Component()
{
	std::cout << "	~Component" << _selfID << std::endl;
}

void Component::SetEntity(Entity* e)
{
	_entity = e;
}

void Component::Awake()
{
}

void Component::Destroy()
{
}

void Component::TransformChange()
{
}

void Component::Tick()
{
	std::cout << "Component  Tick Tick" << std::endl;
}
