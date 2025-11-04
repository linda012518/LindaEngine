#pragma once

namespace LindaEngine
{
#define DECLARE_BEHAVIOR(className) \
	className(Entity& entity, bool enable = true); \
    ~className(); \
    void Initialize(); \
    bool Serialize(); \
    bool Deserialize(YAML::Node& node);
}
