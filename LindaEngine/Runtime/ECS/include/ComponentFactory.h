#pragma once

#include "AutoPtr.h"

#include <unordered_map>
#include <string>
#include <functional>

namespace LindaEngine
{
	class Component;
	class Entity;

	class ComponentFactory
	{
	public:
		//using ComponentCreator = std::function<Ref<Component>(Entity&, bool)>;

		static int RegisterObj(const std::string& className, std::function<Ref<Component>(Entity&, bool)> objCreator);

		static Ref<Component> CreateComponent(const std::string& className, Entity& entity, bool enable = true);

	private:
		static std::unordered_map<std::string, std::function<Ref<Component>(Entity&, bool)>> _componentMap;


	};
}
