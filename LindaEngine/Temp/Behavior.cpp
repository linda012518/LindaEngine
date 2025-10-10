#include "Behavior.h"

//void Component::CheckState()
//{
//	if (_dirty == false)
//		return;
//	_dirty = false;
//
//	OnAwake();
//	OnEnable();
//	OnDisable();
//	OnStart();
//}
//
//bool Component::PrepareAwake()
//{
//	if (_awaked == true || _entity.IsActive() == false)
//		return true;
//
//	return false;
//}
//
//bool Component::PrepareStart()
//{
//	if (_started == true || _enable == false || _entity.IsActive() == false)
//		return true;
//
//	return false;
//}
//
//bool Component::OnAwake()
//{
//	if (_awaked == true || _entity.IsActive() == false)
//		return true;
//
//	_awaked = true;
//	//在黑名单唤醒，如果没有启用状态，也标记会多检查一轮，在白名单唤醒不会
//	if (false == _enable)
//	{
//		OnEnable();
//		OnDisable();
//		OnStart();
//	}
//	//SetDirty();
//
//	return false;
//}
//
//bool Component::OnStart()
//{
//	if (_started == true || _enable == false || _entity.IsActive() == false)
//		return true;
//
//	_started = true;
//
//	return false;
//}
