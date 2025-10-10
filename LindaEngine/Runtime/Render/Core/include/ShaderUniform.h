#pragma once

#include <string>

namespace LindaEngine
{
	enum class UniformType
	{
		TEXTURE, BOOL, 
		INT, INTARRAY, INT2, INT2ARRAY, INT3, INT3ARRAY, INT4, INT4ARRAY, 
		FLOAT, FLOATARRAY, FLOAT2, FLOAT2ARRAY, FLOAT3, FLOAT3ARRAY, FLOAT4, FLOAT4ARRAY, 
		MAT2, MAT2ARRAY, MAT3, MAT3ARRAY, MAT4, MAT4ARRAY
	};

	struct ShaderUniform
	{
		std::string name;
		UniformType type;
		int address;
		int arraySize;

		union UniformData {
			const char* textureUrl;
			bool boolValue;
			int intValue;
			float floatValue;
			int* intArray;
			float* floatArray;
		};
		UniformData data;

		void SetData(void* value)
		{
			switch (type)
			{
			case LindaEngine::UniformType::TEXTURE:
				data.textureUrl = (const char*)value;
				break;
			case LindaEngine::UniformType::BOOL:
				data.boolValue = *((bool*)value);
				break;
			case LindaEngine::UniformType::INT:
				data.intValue = *((int*)value);
				break;
			case LindaEngine::UniformType::INTARRAY:
			case LindaEngine::UniformType::INT2:
			case LindaEngine::UniformType::INT2ARRAY:
			case LindaEngine::UniformType::INT3:
			case LindaEngine::UniformType::INT3ARRAY:
			case LindaEngine::UniformType::INT4:
			case LindaEngine::UniformType::INT4ARRAY:
				data.intArray = (int*)value;
				break;
			case LindaEngine::UniformType::FLOAT:
				data.floatValue = *((float*)value);
				break;
			case LindaEngine::UniformType::FLOATARRAY:
			case LindaEngine::UniformType::FLOAT2:
			case LindaEngine::UniformType::FLOAT2ARRAY:
			case LindaEngine::UniformType::FLOAT3:
			case LindaEngine::UniformType::FLOAT3ARRAY:
			case LindaEngine::UniformType::FLOAT4:
			case LindaEngine::UniformType::FLOAT4ARRAY:
			case LindaEngine::UniformType::MAT2:
			case LindaEngine::UniformType::MAT2ARRAY:
			case LindaEngine::UniformType::MAT3:
			case LindaEngine::UniformType::MAT3ARRAY:
			case LindaEngine::UniformType::MAT4:
			case LindaEngine::UniformType::MAT4ARRAY:
				data.floatArray = (float*)value;
				break;
			}
		}
	};
}
