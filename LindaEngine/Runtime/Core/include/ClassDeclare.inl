#pragma once

#define DECLARE_DYNAMIC_CREATE_CLASS(className) \
    static Ref<className> CreateObject(); \
    std::string GetClassName();