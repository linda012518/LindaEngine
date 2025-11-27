#pragma once

#include "ClassFactory.h"

#define DYNAMIC_CREATE_CLASS(CLASS_NAME, BaseClass) \
    Ref<CLASS_NAME> CLASS_NAME::CreateObject() \
    { \
        return CreateRef<CLASS_NAME>(); \
    } \
    \
    std::string CLASS_NAME::GetClassName() \
    { \
        return #CLASS_NAME; \
    } \
    \
    static int g_tmp_##CLASS_NAME = ClassFactory<BaseClass>::RegisterObj(#CLASS_NAME, CLASS_NAME::CreateObject);
