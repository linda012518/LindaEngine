#pragma once

#include <type_traits>

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
// ����ָ���Ǻ�������ʱ�ò���
//template <typename T> \
//struct Has##func<T, std::void_t<decltype(std::declval<T>().func())>> { \
//    static constexpr bool value = true; \
//};

DEFINE_HAS_MEMBER_FUNCTION(Update)
DEFINE_HAS_MEMBER_FUNCTION(LateUpdate)
DEFINE_HAS_MEMBER_FUNCTION(OnPreRender)
DEFINE_HAS_MEMBER_FUNCTION(OnEndRender)
DEFINE_HAS_MEMBER_FUNCTION(OnEvent)