#include "LifeCycleFuncSystem.h"
#include "Component.h"

using namespace LindaEngine;

//std::list<Component*> LifeCycleFuncSystem::_componentAwake;
//std::list<Component*> LifeCycleFuncSystem::_componentStart;
//
//void LifeCycleFuncSystem::AddComponentAwake(Component* com)
//{
//	_componentAwake.push_back(com);
//}
//
//void LifeCycleFuncSystem::DoComponentAwake()
//{
//    if (_componentAwake.size() <= 0)
//        return;
//
//    for (auto it = _componentAwake.begin(); it != _componentAwake.end(); ) {
//        auto ptr = (*it);
//        if (ptr->PrepareAwake()) {
//            ptr->OnAwake();
//            it = _componentAwake.erase(it);
//        }
//        else {
//            ++it;
//        }
//    }
//}
//
//void LifeCycleFuncSystem::AddComponentStart(Component* com)
//{
//	_componentStart.push_back(com);
//}
//
//void LifeCycleFuncSystem::DoComponentStart()
//{
//    if (_componentStart.size() <= 0)
//        return;
//
//    for (auto it = _componentStart.begin(); it != _componentStart.end(); ) {
//        auto ptr = *it;
//        if (ptr->PrepareStart()) {
//            ptr->OnStart();
//            it = _componentStart.erase(it);
//        }
//        else {
//            ++it;
//        }
//    }
//}
//
//void LifeCycleFuncSystem::RemoveComponent(Component* com)
//{
//    auto itr = std::find(_componentStart.begin(), _componentStart.end(), com);
//    if (itr != _componentStart.end())
//    {
//        _componentStart.erase(itr);
//    }
//
//    auto itr = std::find(_componentAwake.begin(), _componentAwake.end(), com);
//    if (itr != _componentAwake.end())
//    {
//        _componentAwake.erase(itr);
//    }
//}
