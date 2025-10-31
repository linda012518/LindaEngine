#pragma once

#include <string>
#include <glm/glm.hpp>

namespace LindaEngine
{
	enum class UniformType
	{
		None, TEXTURE, INT, INTARRAY, INT4, INT4ARRAY, FLOAT, FLOATARRAY, FLOAT4, FLOAT4ARRAY, 
		MAT2, MAT2ARRAY, MAT3, MAT3ARRAY, MAT4, MAT4ARRAY
	};

	struct ShaderUniform
	{
		std::string name;
		UniformType dataType = UniformType::None;

		virtual ~ShaderUniform() = default;
	};

	struct TextureUniformData : public ShaderUniform
	{
		TextureUniformData() { dataType = UniformType::TEXTURE; }
		std::string value;
	};

	struct FloatUniformData : public ShaderUniform
	{
		FloatUniformData() { dataType = UniformType::FLOAT; }
		float value = 0.0f;
	};

	struct FloatArrayUniformData : public ShaderUniform
	{
		FloatArrayUniformData() { dataType = UniformType::FLOATARRAY; }
		float* value = nullptr;
		int arraySize = 0;
	};

	struct Float4UniformData : public ShaderUniform
	{
		Float4UniformData() { dataType = UniformType::FLOAT4; }
		glm::vec4 value = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	};

	struct Float4ArrayUniformData : public ShaderUniform
	{
		Float4ArrayUniformData() { dataType = UniformType::FLOAT4ARRAY; }
		glm::vec4* value = nullptr;
		int arraySize = 0;
	};

	struct IntUniformData : public ShaderUniform
	{
		IntUniformData() { dataType = UniformType::INT; }
		int value = 0;
	};

	struct IntArrayUniformData : public ShaderUniform
	{
		IntArrayUniformData() { dataType = UniformType::INTARRAY; }
		int* value = nullptr;
		int arraySize = 0;
	};

	struct Int4UniformData : public ShaderUniform
	{
		Int4UniformData() { dataType = UniformType::INT4; }
		glm::ivec4 value = glm::ivec4(0, 0, 0, 0);
	};

	struct Int4ArrayUniformData : public ShaderUniform
	{
		Int4ArrayUniformData() { dataType = UniformType::INT4ARRAY; }
		glm::ivec4* value = nullptr;
		int arraySize = 0;
	};

	struct Mat2UniformData : public ShaderUniform
	{
		Mat2UniformData() { dataType = UniformType::MAT2; }
		glm::mat2 value = glm::mat2(1.0);
	};

	struct Mat2ArrayUniformData : public ShaderUniform
	{
		Mat2ArrayUniformData() { dataType = UniformType::MAT2ARRAY; }
		glm::mat2* value = nullptr;
		int arraySize = 0;
	};

	struct Mat3UniformData : public ShaderUniform
	{
		Mat3UniformData() { dataType = UniformType::MAT3; }
		glm::mat3 value = glm::mat3(1.0);
	};

	struct Mat3ArrayUniformData : public ShaderUniform
	{
		Mat3ArrayUniformData() { dataType = UniformType::MAT3ARRAY; }
		glm::mat3* value = nullptr;
		int arraySize = 0;
	};

	struct Mat4UniformData : public ShaderUniform
	{
		Mat4UniformData() { dataType = UniformType::MAT4; }
		glm::mat4 value = glm::mat4(1.0);
	};

	struct Mat4ArrayUniformData : public ShaderUniform
	{
		Mat4ArrayUniformData() { dataType = UniformType::MAT4ARRAY; }
		glm::mat4* value = nullptr;
		int arraySize = 0;
	};

	//struct ShaderUniform
	//{
	//	std::string name;
	//	UniformType type;
	//	int arraySize;

	//	union UniformData {
	//		const char* textureUrl;
	//		bool boolValue;
	//		int intValue;
	//		float floatValue;
	//		int* intArray;
	//		float* floatArray;
	//	};
	//	UniformData data;

	//	void SetData(void* value)
	//	{
	//		switch (type)
	//		{
	//		case LindaEngine::UniformType::TEXTURE:
	//			data.textureUrl = (const char*)value;
	//			break;
	//		case LindaEngine::UniformType::INT:
	//			data.intValue = *((int*)value);
	//			break;
	//		case LindaEngine::UniformType::INTARRAY:
	//		case LindaEngine::UniformType::INT4:
	//		case LindaEngine::UniformType::INT4ARRAY:
	//			data.intArray = (int*)value;
	//			break;
	//		case LindaEngine::UniformType::FLOAT:
	//			data.floatValue = *((float*)value);
	//			break;
	//		case LindaEngine::UniformType::FLOATARRAY:
	//		case LindaEngine::UniformType::FLOAT4:
	//		case LindaEngine::UniformType::FLOAT4ARRAY:
	//		case LindaEngine::UniformType::MAT2:
	//		case LindaEngine::UniformType::MAT2ARRAY:
	//		case LindaEngine::UniformType::MAT3:
	//		case LindaEngine::UniformType::MAT3ARRAY:
	//		case LindaEngine::UniformType::MAT4:
	//		case LindaEngine::UniformType::MAT4ARRAY:
	//			data.floatArray = (float*)value;
	//			break;
	//		}
	//	}
	//};
}
