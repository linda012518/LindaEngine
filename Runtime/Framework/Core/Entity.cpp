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
	std::cout << "	Entity()" << _selfID << std::endl;
}

Entity::Entity(const char* name)
{
	_name = name;
	_transform = CreateRef<Transform>(this);
	std::cout << "	Entity" << _selfID << std::endl;
}

Entity::~Entity()
{
	std::cout << "	~Entity" << _selfID << std::endl;
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

void Entity::TransformChange()
{
	for (int i = 0; i < _components.size(); i++) {
		_components[i]->TransformChange();
	}
}

Ref<Component> Entity::AddComponent(const std::string& className)
{
	return nullptr;
}
