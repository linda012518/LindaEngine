#include "ComponentFactory.h"
#include "Component.h"

using namespace LindaEngine;

//std::unordered_map<std::string, std::function<Ref<Component>(Entity&, bool)>> ComponentFactory::_componentMap;

int ComponentFactory::RegisterObj(const std::string& className, std::function<Ref<Component>(Entity&, bool)> objCreator)
{
    GetMap()[className] = objCreator;
    CollectComponents(className);
    return 0;
}

Ref<Component> ComponentFactory::CreateComponent(const std::string& className, Entity& entity, bool enable)
{
    auto& map = GetMap();
    auto it = map.find(className);
    if (it == map.end())
    {
        return nullptr;
    }
    return it->second(entity, enable);
}

std::unordered_map<std::string, std::function<Ref<Component>(Entity&, bool)>>& ComponentFactory::GetMap()
{
    static std::unordered_map<std::string, std::function<Ref<Component>(Entity&, bool)>> map;
    return map;
}

std::vector<std::string>& ComponentFactory::GetComponents()
{
    static std::vector<std::string> components;
    return components;
}

void ComponentFactory::CollectComponents(const std::string& className)
{
    GetComponents().push_back(className);
}


