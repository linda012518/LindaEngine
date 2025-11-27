#pragma once

#include "AutoPtr.h"

#include <unordered_map>
#include <string>
#include <functional>

namespace LindaEngine
{
	template<typename BaseClass>
	class ClassFactory
	{
	public:
		static int RegisterObj(const std::string& className, std::function<Ref<BaseClass>()> objCreator)
		{
			GetMap()[className] = objCreator;
			return 0;
		}

		static Ref<BaseClass> CreateObj(const std::string& className)
		{
			auto& map = GetMap();
			auto it = map.find(className);
			if (it == map.end())
			{
				return nullptr;
			}
			return it->second();
		}

	private:
		static std::unordered_map<std::string, std::function<Ref<BaseClass>()>>& GetMap()
		{
			static std::unordered_map<std::string, std::function<Ref<BaseClass>()>> map;
			return map;
		}
	};
}
