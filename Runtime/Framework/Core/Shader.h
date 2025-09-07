#pragma once

#include "LObject.hpp"
#include "glad/glad.h"
#include "glm/glm.hpp"

#include <unordered_map>

namespace LindaEngine
{
	enum    ShaderType
	{
		SHADER_NONE		=	0,
		SHADER_VERTEX	=	GL_VERTEX_SHADER,			//����
		SHADER_TES		=	GL_TESS_EVALUATION_SHADER,	//����ϸ��1
		SHADER_TCS		=	GL_TESS_CONTROL_SHADER,		//����ϸ��2
		SHADER_GEOMETRY =	GL_GEOMETRY_SHADER,			//����
		SHADER_FRAGMENT =	GL_FRAGMENT_SHADER,			//ƬԪ
		SHADER_COMPUTE	=	GL_COMPUTE_SHADER,			//����
	};

	typedef int uniform;

	class Shader : public LObject
	{
	public:
		Shader();
		Shader(const char* vs, const char* fs);
		virtual ~Shader();

		void Begin();

		void End();

		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetIntArray(const std::string& name, int count, int* value);
		void SetFloatArray(const std::string& name, int count, float* value);

		void SetVec2(const std::string& name, const glm::vec2& value);
		void SetVec2Array(const std::string& name, int count, glm::vec2* value);

		void SetVec3(const std::string& name, const glm::vec3& value);
		void SetVec3Array(const std::string& name, int count, glm::vec3* value);

		void SetVec4(const std::string& name, const glm::vec4& value);
		void SetVec4Array(const std::string& name, int count, glm::vec4* value);

		void SetMat2(const std::string& name, const glm::mat2& mat);
		void SetMat2Array(const std::string& name, int count, const glm::mat2* mat);

		void SetMat3(const std::string& name, const glm::mat3& mat);
		void SetMat3Array(const std::string& name, int count, const glm::mat3* mat);

		void SetMat4(const std::string& name, const glm::mat4& mat);
		void SetMat4Array(const std::string& name, int count, const glm::mat4* mat);

	private:
		GLint	_program = -1;

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
