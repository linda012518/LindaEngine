#include "ComponentFactory.h"
#include "Component.h"

using namespace LindaEngine;

std::unordered_map<std::string, std::function<Ref<Component>(Entity&, bool)>> ComponentFactory::_componentMap;

int ComponentFactory::RegisterObj(const std::string& className, std::function<Ref<Component>(Entity&, bool)> objCreator)
{
	_componentMap[className] = objCreator;
    return 0;
}

Ref<Component> ComponentFactory::CreateComponent(const std::string& className, Entity& entity, bool enable)
{
    auto it = _componentMap.find(className);
    if (it == _componentMap.end())
    {
        return nullptr;
    }
    return it->second(entity, enable);
}


