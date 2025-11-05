#include "Component.h"
#include "Entity.h"

using namespace LindaEngine;

Component::Component(Entity& entity, bool enable) :
	_enable(enable), _entity(entity)
{
	_enableDirty = false;
	_enableLast = _enable;
	_transform = _entity.GetTransform();
}

Component::~Component()
{
}

Entity& Component::GetEntity() const
{
	return _entity;
}

bool Component::IsActive() const
{
	return _entity.IsActive();
}

bool Component::IsDirty()
{
	bool result = _entity.IsDirty() || _enableDirty;
	return result;
}

void Component::ClearDirty()
{
	_entity.ClearDirty();
	_enableDirty = false;
}

bool Component::IsEnable() const
{
	return _enable && _entity.IsActive();
}

bool Component::LastEnbale()
{
	return _enableLast;
}

void Component::SetEnable(bool enable)
{
	if (_enable == enable)
		return;
	_enable = enable;
	_enableDirty = true;
	_enableLast = !_enable;
}


