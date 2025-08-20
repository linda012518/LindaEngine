#include "Interface/IDefined.h"
#include "Entity.hpp"

#include <string.h>

using namespace LindaEngine;

Entity::Entity()
{
	std::cout << "Entity" << std::endl;
}

Entity::Entity(const char* name)
{
	_name = name;
	std::cout << "Entity" << std::endl;
}

Entity::~Entity()
{
	std::cout << "~Entity" << std::endl;
}

void Entity::setName(const std::string& name)
{
	_name = name;
}

std::string& Entity::getName()
{
	return _name;
}

Ref<Component> Entity::AddComponent(const std::string& className)
{
	return nullptr;
}
