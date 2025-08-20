#include "Component.hpp"

using namespace LindaEngine;

Component::Component()
{
	std::cout << "Component" << std::endl;
}

Component::~Component()
{
	std::cout << "~Component" << std::endl;
}

void Component::SetEntity(Ref<Entity>& e)
{
	_entity = e;
}
