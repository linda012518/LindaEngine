#pragma once

#include <memory>
#include "MacroDefined.h"

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

template<typename T>
class Weak
{
	// 让所有 Weak<U> 能访问私有成员，子类隐式转换时需要访问父类的私有成员
	template<typename> friend class Weak;

public:
	Weak() = default;
	Weak(std::nullptr_t) {}
	Weak(const std::shared_ptr<T>& ptr) : _weak(ptr) {}
	Weak(const std::weak_ptr<T>& ptr) : _weak(ptr) {}
	template<typename U>
	Weak(const std::shared_ptr<U>& ptr) : _weak(ptr) {}
	template<typename U>
	Weak(const std::weak_ptr<U>& ptr) : _weak(ptr) {}
	template<typename U>
	Weak(const Weak<U>& other) : _weak(other._weak) {}

	T* operator->() const {
		auto locked = _weak.lock();
		return locked ? locked.get() : nullptr;
	}
	void operator=(const std::shared_ptr<T>& ptr) { _weak = ptr; }

	bool operator==(const Weak& other) const {
		return !_weak.owner_before(other._weak) && !other._weak.owner_before(_weak);
	}
	//bool operator!=(const Weak& other) const {
	//	return !(*this == other);
	//}

	bool operator==(std::nullptr_t) const {
		return _weak.expired();
	}
	bool operator!=(std::nullptr_t) const {
		return !_weak.expired();
	}

	explicit operator bool() const { 
		return !_weak.expired(); 
	}

	bool Expired() const { return _weak.expired(); }
	std::shared_ptr<T> Lock() const { return _weak.lock(); }

private:
	std::weak_ptr<T> _weak;
};

// 非成员：支持 nullptr == weak
template<typename T>
inline bool operator==(std::nullptr_t, const Weak<T>& w) { return w.Expired(); }

template<typename T>
inline bool operator!=(std::nullptr_t, const Weak<T>& w) { return !w.Expired(); }

#define StaticCastRef(type, pointer) std::static_pointer_cast<type>(pointer)
#define DynamicCastRef(type, pointer) std::dynamic_pointer_cast<type>(pointer)
#define DynamicCastWeak(type, pointer) Weak<type>(std::dynamic_pointer_cast<type>(pointer.Lock()))

