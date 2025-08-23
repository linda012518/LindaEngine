#include "Interface/IDefined.h"
#include "Entity.hpp"
#include "Component/Transform.h"

#include <string.h>

using namespace LindaEngine;

Entity::Entity()
{
	_transform = AddComponent<Transform>();
	std::cout << "Entity()" << std::endl;
}

Entity::Entity(const char* name)
{
	_name = name;
	_transform = AddComponent<Transform>();
	std::cout << "Entity(name)" << std::endl;
}

Entity::~Entity()
{
	std::cout << "~Entity" << std::endl;
}

void Entity::setName(const std::string& name)
{
	_name = name;
}

const std::string& Entity::getName() const
{
	return _name;
}

Ref<Component> Entity::AddComponent(const std::string& className)
{
	return nullptr;
}
