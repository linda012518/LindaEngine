#pragma once

#define DECLARE_BEHAVIOR(className) \
    DECLARE_DYNAMIC_CREATE() \
	className(Entity& entity, bool enable = true); \
    virtual ~className(); \
    void Initialize(); \
    bool Serialize(); \
    bool Deserialize(YAML::Node& node);

