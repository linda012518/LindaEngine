#pragma once

#define DECLARE_DYNAMIC_CREATE() \
    static Ref<Component> CreateComponent(Entity& entity, bool enable = true); \
    std::string GetClassName() const override;
