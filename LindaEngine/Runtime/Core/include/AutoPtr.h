#pragma once

#include <memory>
#include "MacroDefined.h"

namespace LindaEngine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) //constexpr 将运算尽量放在编译阶段
	{
		//独占指针 std::move(ptr)
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) //constexpr 将运算尽量放在编译阶段
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	#define CastAutoPointer(type, pointer) std::shared_ptr<type>(pointer)
	#define StaticCastRef(type, pointer) std::static_pointer_cast<type>(pointer);
	#define DynamicCastRef(type, pointer) std::dynamic_pointer_cast<type>(pointer);
	
}