#include "Component.h"
#include "Entity.h"

using namespace LindaEngine;

Component::Component(Entity& entity, bool enable) :
	_enable(enable), _entity(entity), _dirty(false), _awaked(false), _started(false)
{

}

Component::~Component()
{
}

Entity& Component::GetEntity() const
{
	return _entity;
}

bool Component::GetEnable() const
{
	return _enable;
}

void Component::SetEnable(bool enable)
{
	if (_enable == enable)
		return;
	_enable = enable;
	SetDirty();
}

void Component::SetDirty()
{
	_dirty = true;
}

bool Component::OnAwake()
{
	if (_awaked == true || _entity.IsActive() == false)
		return true;

	_awaked = true;
	//在黑名单唤醒，如果没有启用状态，也标记会多检查一轮，在白名单唤醒不会
	if (false == _enable)
		SetDirty();

	return false;
}

bool Component::OnStart()
{
	if (_started == true || _enable == false || _entity.IsActive() == false)
		return true;

	_started = true;

	return false;
}
