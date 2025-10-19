#pragma once

#include "ComponentFactory.h"

namespace LindaEngine
{

#define DECLARE_DYNAMIC_CREATE(CLASS_NAME) \
    static Ref<Component> CreateComponent(Entity& entity, bool enable = true); \
    std::string GetClassName() const override;

#define DYNAMIC_CREATE(CLASS_NAME) \
    Ref<Component> CLASS_NAME::CreateComponent(Entity& entity, bool enable) \
    { \
        return CreateRef<CLASS_NAME>(entity, enable); \
    } \
    \
    std::string CLASS_NAME::GetClassName() const \
    { \
        return #CLASS_NAME; \
    } \
    \
    static int g_tmp_##CLASS_NAME = \
        ComponentFactory::RegisterObj(#CLASS_NAME, CLASS_NAME::CreateComponent);


    //template<typename T>
    //T* CREATE_OBJECT(const std::string& className, Entity& entity, bool enable)
    //{
    //    return dynamic_cast<T*>(ComponentFactory::CreateComponent(className, entity, enable));
    //}
}
