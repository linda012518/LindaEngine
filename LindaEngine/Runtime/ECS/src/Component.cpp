#include "Component.h"
#include "Entity.h"

using namespace LindaEngine;

Component::Component(Entity& entity, bool enable) :
	_enable(enable), _entity(entity)
{

}

Component::~Component()
{
}

Entity& Component::GetEntity() const
{
	return _entity;
}

bool Component::IsEnable() const
{
	return _enable && _entity.IsActive();
}

void Component::SetEnable(bool enable)
{
	if (_enable == enable)
		return;
	_enable = enable;
}

