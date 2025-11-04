#include "EventSystem.h"
#include "LObject.h"

using namespace LindaEngine;

std::unordered_map<int, std::list<IEventHandler*>> EventSystem::_eventMap;

void EventSystem::Bind(int code, IEventHandler* obj)
{
    auto it1 = _eventMap.find(code);
    if (it1 != _eventMap.end())
    {
        std::list<IEventHandler*>& go = _eventMap[code];

        auto it2 = std::find(go.begin(), go.end(), obj);
        if (it2 == go.end())
        {
            go.push_back(obj);
        }
    }
    else
    {
        std::list<IEventHandler*> go;
        go.push_back(obj);
        _eventMap[code] = go;
    }
}

void EventSystem::Unbind(int code, IEventHandler* obj)
{
    auto it1 = _eventMap.find(code);
    if (it1 == _eventMap.end())
        return;

    std::list<IEventHandler*>& go = _eventMap[code];

    auto it2 = std::find(go.begin(), go.end(), obj);
    if (it2 != go.end())
    {
        go.erase(it2);
    }
}

void EventSystem::Dispatch(IEventHandler* sender, int code, Event& eventData)
{
    auto itr = _eventMap.find(code);
    if (itr == _eventMap.end())
        return;

    std::list<IEventHandler*> go = _eventMap[code];

    for (auto it = go.begin(); it != go.end(); ++it) {
        IEventHandler* temp = *it;
        if (nullptr != temp)
            temp->OnEvent(sender, code, eventData);
    }
}
