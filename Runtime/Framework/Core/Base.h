#pragma once

#include <memory>

namespace LindaEngine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) //constexpr �����㾡�����ڱ���׶�
	{
		//��ռָ�� std::move(ptr)
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) //constexpr �����㾡�����ڱ���׶�
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}