#include "EventSystem.h"
#include "LObject.h"
#include "Entity.h"
#include "Component.h"

using namespace LindaEngine;

std::unordered_map<int, std::list<Weak<IEventHandler>>> EventSystem::_eventMap;

void EventSystem::Bind(int code, Weak<IEventHandler> obj)
{
    auto it1 = _eventMap.find(code);
    if (it1 != _eventMap.end())
    {
        std::list<Weak<IEventHandler>>& go = _eventMap[code];

        auto it2 = std::find(go.begin(), go.end(), obj);
        if (it2 == go.end())
        {
            go.push_back(obj);
        }
    }
    else
    {
        std::list<Weak<IEventHandler>> go;
        go.push_back(obj);
        _eventMap[code] = go;
    }
}

void EventSystem::Unbind(int code, Weak<IEventHandler> obj)
{
    auto it1 = _eventMap.find(code);
    if (it1 == _eventMap.end())
        return;

    std::list<Weak<IEventHandler>>& go = _eventMap[code];

    auto it2 = std::find(go.begin(), go.end(), obj);
    if (it2 != go.end())
    {
        go.erase(it2);
    }
}

void EventSystem::Dispatch(Weak<IEventHandler> sender, int code, Event& eventData)
{
    auto itr = _eventMap.find(code);
    if (itr == _eventMap.end())
        return;

    std::list<Weak<IEventHandler>> go = _eventMap[code];

    for (auto it = go.begin(); it != go.end(); ++it) {

        Weak<IEventHandler> temp = *it;
        if (temp)
        {
            Weak<Component> com = DynamicCastWeak(Component, temp);
            if (nullptr != com && false == com->IsEnable())
                continue;
            temp->OnEvent(sender, code, eventData);
        }
    }
}

void EventSystem::Clear()
{
    std::unordered_map<int, std::list<Weak<IEventHandler>>> temp = _eventMap;

    _eventMap.clear();

    for (auto& dic : temp)
    {
        for (auto& obj : dic.second)
        {
            Weak<Entity> entity = DynamicCastWeak(Entity, obj);
            if (nullptr != entity && entity->GetDontDestory())
                Bind(dic.first, obj);

            Weak<Component> com = DynamicCastWeak(Component, obj);
            if (nullptr != com && com->GetEntity().GetDontDestory())
                Bind(dic.first, obj);
        }
    }
}
