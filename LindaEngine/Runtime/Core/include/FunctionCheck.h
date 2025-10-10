#pragma once

#include <type_traits>

//检查是否有指定函数和成员  HasUpdate<T>::value

#define DEFINE_HAS_MEMBER_FUNCTION(func) \
template <typename T, typename = void> \
struct Has##func { \
    static constexpr bool value = false; \
}; \
\
template <typename T> \
struct Has##func<T, std::void_t<decltype(&T::func)>> { \
    static constexpr bool value = true; \
}; 
// 可以指定是函数，暂时用不到
//template <typename T> \
//struct Has##func<T, std::void_t<decltype(std::declval<T>().func())>> { \
//    static constexpr bool value = true; \
//};

DEFINE_HAS_MEMBER_FUNCTION(Update)
DEFINE_HAS_MEMBER_FUNCTION(OnPreRender)
DEFINE_HAS_MEMBER_FUNCTION(OnPostRender)
DEFINE_HAS_MEMBER_FUNCTION(LateUpdate)
DEFINE_HAS_MEMBER_FUNCTION(OnEvent)

//检查是否重写父类函数  
#define OVERRIDE(baseType, DerivedType, func) std::is_same<decltype(&DerivedType::func), decltype(&baseType::func)>::value
