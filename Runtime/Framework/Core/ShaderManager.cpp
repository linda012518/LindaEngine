#include "ShaderManager.h"

using namespace LindaEngine;

void ShaderManager::AddShaderSource(std::string& path, std::string& vert, std::string frag)
{
    auto itr = _map.find(path);
    if (itr != _map.end())
    {
        ShaderSource ss;
        ss.vertex = vert;
        ss.fragment = frag;
        _map[path] = ss;
    }
}
