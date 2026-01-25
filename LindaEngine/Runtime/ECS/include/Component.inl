#pragma once

#define DECLARE_DYNAMIC_CREATE() \
    static Ref<LindaEngine::Component> CreateComponent(LindaEngine::Entity& entity, bool enable = true); \
    std::string GetClassName() const override;
