#pragma once

#include <stdint.h>
#include <string>
#include <unordered_map>

namespace LindaEngine
{
	enum class ShaderDataType
	{
		None, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat2, Mat3, Mat4, Bool, Bool2, Bool3, Bool4
	};

	enum class AttributeType
	{
		Position, Color, Normal, Tangent, UV0, UV1, UV2, UV3, UV4, UV5, UV6, UV7, EntityID
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		return 0;
	}

	static ShaderDataType GetShaderDataType(std::string& type)
	{
		//switch (type)
		//{
		//case ShaderDataType::Float:    return 4;
		//case ShaderDataType::Float2:   return 4 * 2;
		//case ShaderDataType::Float3:   return 4 * 3;
		//case ShaderDataType::Float4:   return 4 * 4;
		//case ShaderDataType::Mat3:     return 4 * 3 * 3;
		//case ShaderDataType::Mat4:     return 4 * 4 * 4;
		//case ShaderDataType::Int:      return 4;
		//case ShaderDataType::Int2:     return 4 * 2;
		//case ShaderDataType::Int3:     return 4 * 3;
		//case ShaderDataType::Int4:     return 4 * 4;
		//case ShaderDataType::Bool:     return 1;
		//}

		return ShaderDataType::None;
	}

	struct ShaderAttribute
	{
		AttributeType attribute;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		uint32_t index;
		bool normalized;

		ShaderAttribute(AttributeType attr, std::string dataType, bool normalizeded = false)
		{
			attribute = attr;
			type = _mapToShaderDataType[dataType];
			size = ShaderDataTypeSize(type);
			normalized = normalizeded;
		}

	private:
		static std::unordered_map<std::string, ShaderDataType> _mapToShaderDataType;
	};
}
