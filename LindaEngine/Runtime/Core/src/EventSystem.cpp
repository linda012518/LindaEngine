#include "EventSystem.h"
#include "LObject.h"

#define IMPLEMENT_EVENTSYSTEM(SUFFIX) \
 \
std::unordered_map<int, std::list<IEventHandler*>> EventSystem::_eventMap##SUFFIX; \
 \
void EventSystem::Bind##SUFFIX(int code, IEventHandler* obj) \
{ \
    auto it1 = _eventMap##SUFFIX.find(code); \
    if (it1 != _eventMap##SUFFIX.end()) \
    { \
        std::list<IEventHandler*>& go = _eventMap##SUFFIX[code]; \
 \
        auto it2 = std::find(go.begin(), go.end(), obj); \
        if (it2 == go.end()) \
        { \
            go.push_back(obj); \
        } \
    } \
    else \
    { \
        std::list<IEventHandler*> go; \
        go.push_back(obj); \
        _eventMap##SUFFIX[code] = go; \
    } \
} \
 \
void EventSystem::Unbind##SUFFIX(int code, IEventHandler* obj) \
{ \
    auto it1 = _eventMap##SUFFIX.find(code); \
    if (it1 == _eventMap##SUFFIX.end()) \
        return; \
 \
    std::list<IEventHandler*>& go = _eventMap##SUFFIX[code]; \
 \
    auto it2 = std::find(go.begin(), go.end(), obj); \
    if (it2 != go.end()) \
    { \
        go.erase(it2); \
    } \
} \
 \
void EventSystem::Dispatch##SUFFIX(IEventHandler* sender, int code, Event& eventData) \
{ \
    auto itr = _eventMap##SUFFIX.find(code); \
    if (itr == _eventMap##SUFFIX.end()) \
        return; \
 \
    std::list<IEventHandler*> go = _eventMap##SUFFIX[code]; \
 \
    for (auto it = go.begin(); it != go.end(); ++it) { \
        IEventHandler* temp = *it; \
        if (nullptr != temp) \
            temp->OnEvent(sender, code, eventData); \
    } \
} \
 \
void EventSystem::Clear##SUFFIX() \
{ \
    _eventMap##SUFFIX.clear(); \
}



using namespace LindaEngine;

IMPLEMENT_EVENTSYSTEM()

IMPLEMENT_EVENTSYSTEM(Editor)

//std::unordered_map<int, std::list<IEventHandler*>> EventSystem::_eventMap;
//
//void EventSystem::Bind(int code, IEventHandler* obj)
//{
//    auto it1 = _eventMap.find(code);
//    if (it1 != _eventMap.end())
//    {
//        std::list<IEventHandler*>& go = _eventMap[code];
//
//        auto it2 = std::find(go.begin(), go.end(), obj);
//        if (it2 == go.end())
//        {
//            go.push_back(obj);
//        }
//    }
//    else
//    {
//        std::list<IEventHandler*> go;
//        go.push_back(obj);
//        _eventMap[code] = go;
//    }
//}
//
//void EventSystem::Unbind(int code, IEventHandler* obj)
//{
//    auto it1 = _eventMap.find(code);
//    if (it1 == _eventMap.end())
//        return;
//
//    std::list<IEventHandler*>& go = _eventMap[code];
//
//    auto it2 = std::find(go.begin(), go.end(), obj);
//    if (it2 != go.end())
//    {
//        go.erase(it2);
//    }
//}
//
//void EventSystem::Dispatch(IEventHandler* sender, int code, Event& eventData)
//{
//    auto itr = _eventMap.find(code);
//    if (itr == _eventMap.end())
//        return;
//
//    std::list<IEventHandler*> go = _eventMap[code];
//
//    for (auto it = go.begin(); it != go.end(); ++it) {
//        IEventHandler* temp = *it;
//        if (nullptr != temp)
//            temp->OnEvent(sender, code, eventData);
//    }
//}
//
//void EventSystem::Clear()
//{
//    _eventMap.clear();
//}
