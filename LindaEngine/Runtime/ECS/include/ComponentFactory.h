#pragma once

#include "AutoPtr.h"

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

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

		static std::vector<std::string>& GetComponents();

	private:
		static std::unordered_map<std::string, std::function<Ref<Component>(Entity&, bool)>>& GetMap();

		static void CollectComponents(const std::string& className);
	//private:
	//	static std::unordered_map<std::string, std::function<Ref<Component>(Entity&, bool)>> _componentMap;


	};
}
