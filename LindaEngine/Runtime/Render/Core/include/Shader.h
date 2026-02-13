#pragma once

#include "LObject.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

#include <unordered_map>

namespace LindaEngine
{
	enum class ShaderType
	{
		SHADER_NONE		=	0,
		SHADER_VERTEX	=	GL_VERTEX_SHADER,			//顶点
		SHADER_TES		=	GL_TESS_EVALUATION_SHADER,	//曲面细分1
		SHADER_TCS		=	GL_TESS_CONTROL_SHADER,		//曲面细分2
		SHADER_GEOMETRY =	GL_GEOMETRY_SHADER,			//几何
		SHADER_FRAGMENT =	GL_FRAGMENT_SHADER,			//片元
		SHADER_COMPUTE	=	GL_COMPUTE_SHADER,			//计算
	};

	class Shader : public LObject
	{
	public:
		Shader();
		Shader(const char* vs, const char* fs);
		virtual ~Shader();

		void Begin();
		void End();
		bool HasError() { return !_noError; }

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetIntArray(const std::string& name, int count, int* value);
		void SetFloatArray(const std::string& name, int count, float* value);

		void SetIVec4(const std::string& name, const glm::ivec4& value, int count = 1);
		void SetVec4(const std::string& name, const glm::vec4& value, int count = 1);
		void SetMat2(const std::string& name, const glm::mat2& mat, int count = 1);
		void SetMat3(const std::string& name, const glm::mat3& mat, int count = 1);
		void SetMat4(const std::string& name, const glm::mat4& mat, int count = 1);

	private:
		GLint	_program = -1;
		bool	_noError = false;

		const char* _vs = nullptr;
		const char* _tes = nullptr;
		const char* _tcs = nullptr;
		const char* _gs = nullptr;
		const char* _fs = nullptr;
		const char* _cs = nullptr;

		std::unordered_map<std::string, GLint> _uniformMap;

	private:
		GLint GetUniform(const std::string& name);

		bool CompileShader();

		void AttachShader(GLuint shader);

		void DettachShader(GLuint shader);

		bool LinkProgram();

		unsigned CreateShader(const char* source, ShaderType type);

		
	};
}
