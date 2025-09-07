#include "Interface/IDefined.h"
#include "Entity.hpp"
#include "Transform.h"
#include "Component.hpp"

#include <string.h>

using namespace LindaEngine;

Entity::Entity()
{
	_name = "LindaEntity";
	_transform = CreateRef<Transform>(this);
	std::cout << "	Entity()" << std::endl;
}

Entity::Entity(const char* name)
{
	_name = name;
	_transform = CreateRef<Transform>(this);
	std::cout << "	Entity(name)" << std::endl;
}

Entity::~Entity()
{
	std::cout << "	~Entity" << std::endl;
}

void Entity::SetName(const std::string& name)
{
	_name = name;
}

const std::string& Entity::GetName() const
{
	return _name;
}

Transform* Entity::GetTransform()
{
	return _transform.get();
}

Ref<Component> Entity::AddComponent(const std::string& className)
{
	return nullptr;
}
