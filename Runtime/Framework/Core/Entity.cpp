#include "Interface/IDefined.h"
#include "Entity.hpp"

#include <string.h>

using namespace LindaEngine;

int Entity::_entityCount = 0;

Entity::Entity()
{
	_id = _entityCount++;
}

Entity::~Entity()
{
}

void Entity::setName(const char* name)
{
	strncpy(_name, name, sizeof(_name));
}

char* Entity::getName()
{
	return _name;
}
