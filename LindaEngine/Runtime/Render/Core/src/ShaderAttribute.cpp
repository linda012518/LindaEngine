#include "ShaderAttribute.h"

using namespace LindaEngine;

std::unordered_map<std::string, ShaderDataType> ShaderAttribute::_mapToShaderDataType = {
	{ "float", ShaderDataType::Float },
	{ "vec2", ShaderDataType::Float2 },
	{ "vec3", ShaderDataType::Float3 },
	{ "vec4", ShaderDataType::Float4 },
	{ "int", ShaderDataType::Int },
	{ "ivec2", ShaderDataType::Int2 },
	{ "ivec3", ShaderDataType::Int3 },
	{ "ivec4", ShaderDataType::Int4 },
	{ "mat2", ShaderDataType::Mat2 },
	{ "mat3", ShaderDataType::Mat3 },
	{ "mat4", ShaderDataType::Mat4 },
	{ "bool", ShaderDataType::Bool },
	{ "bvec2", ShaderDataType::Bool2 },
	{ "bvec3", ShaderDataType::Bool3 },
	{ "bvec4", ShaderDataType::Bool4 }
};
