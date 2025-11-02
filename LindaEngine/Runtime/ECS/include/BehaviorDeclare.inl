#pragma once

namespace LindaEngine
{
#define DECLARE_BEHAVIOR() \
    void Initialize(); \
    bool Serialize(); \
    bool Deserialize(YAML::Node& node);
}
