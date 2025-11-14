#include "Shader.h"

#define IMPLEMENT_UNIFORM(func) \
int addr = GetUniform(name); \
if (addr >= 0) \
{ \
	func; \
}

#define DELETESHADER(handle) \
if (handle != -1) \
	glDeleteShader(handle);

#define CREATESHADER(handle, str, type) \
GLuint handle = -1; \
if (str != nullptr) \
{ \
	handle = CreateShader(str, type); \
	if (handle == -1) \
	{ \
		if (handle != -1) \
			glDeleteShader(handle); \
		return false; \
	} \
}

using namespace LindaEngine;

Shader::Shader()
{
}

Shader::Shader(const char* vs, const char* fs)
{
	_vs = vs;
	_fs = fs;

	CompileShader();
	glUniformBlockBinding(_program, glGetUniformBlockIndex(_program, "globalUniform"), 0);
}

Shader::~Shader()
{
	if (_program != -1)
		glDeleteProgram(_program);
}

void Shader::Begin()
{
	glUseProgram(_program);
}

void Shader::End()
{
	glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value)
{
	IMPLEMENT_UNIFORM(glUniform1i(addr, value))
	//int addr = GetUniform(name);
	//if (addr >= 0)
	//{
	//	glUniform1i(addr, value);
	//}
}

void Shader::SetFloat(const std::string& name, float value)
{
	IMPLEMENT_UNIFORM(glUniform1f(addr, value))
	//int addr = GetUniform(name);
	//if (addr >= 0)
	//{
	//	glUniform1f(addr, value);
	//}
}

void Shader::SetIntArray(const std::string& name, int count, int* value)
{
	IMPLEMENT_UNIFORM(glUniform1iv(addr, count, value))
	//int addr = GetUniform(name);
	//if (addr >= 0)
	//{
	//	glUniform1iv(addr, count, value);
	//}
}

void Shader::SetFloatArray(const std::string& name, int count, float* value)
{
	IMPLEMENT_UNIFORM(glUniform1fv(addr, count, value))
	//int addr = GetUniform(name);
	//if (addr >= 0)
	//{
	//	glUniform1fv(addr, count, value);
	//}
}

void Shader::SetIVec4(const std::string& name, const glm::ivec4& value, int count)
{
	IMPLEMENT_UNIFORM(glUniform4iv(addr, count, &value[0]))
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value, int count)
{
	IMPLEMENT_UNIFORM(glUniform4fv(addr, count, &value[0]))

	//int addr = GetUniform(name);
	//if (addr >= 0)
	//{
	//	glUniform4fv(addr, 1, &value[0]);
	//}
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat, int count)
{
	IMPLEMENT_UNIFORM(glUniformMatrix2fv(addr, count, GL_FALSE, &mat[0][0]))

	//int addr = GetUniform(name);
	//if (addr >= 0)
	//{
	//	glUniformMatrix2fv(addr, 1, GL_FALSE, &mat[0][0]);
	//}
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat, int count)
{
	IMPLEMENT_UNIFORM(glUniformMatrix3fv(addr, count, GL_FALSE, &mat[0][0]))

	//int addr = GetUniform(name);
	//if (addr >= 0)
	//{
	//	glUniformMatrix3fv(addr, 1, GL_FALSE, &mat[0][0]);
	//}
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat, int count)
{
	IMPLEMENT_UNIFORM(glUniformMatrix4fv(addr, count, GL_FALSE, &mat[0][0]))

	//int addr = GetUniform(name);
	//if (addr >= 0)
	//{
	//	glUniformMatrix4fv(addr, 1, GL_FALSE, &mat[0][0]);
	//}
}

GLint Shader::GetUniform(const std::string& name)
{
	if (_uniformMap.find(name) != _uniformMap.end())
		return _uniformMap[name];

	GLint addr = glGetUniformLocation(_program, name.c_str());
	_uniformMap[name] = addr;
	return addr;
}

bool Shader::CompileShader()
{
	_program = glCreateProgram();

	//GLuint vsHandle = -1;
	//GLuint fsHandle = -1;
	//if (_vs != nullptr && _fs != nullptr)
	//{
	//	vsHandle = CreateShader(_vs, SHADER_VERTEX);
	//	fsHandle = CreateShader(_fs, SHADER_FRAGMENT);
	//	if (vsHandle == -1 || fsHandle == -1)
	//	{
	//		DELETESHADER(vsHandle);
	//		DELETESHADER(fsHandle);
	//		return false;
	//	}
	//}

	//GLuint tesHandle = -1;
	//GLuint tcsHandle = -1;
	//if (_tes != nullptr && _tcs != nullptr)
	//{
	//	tesHandle = CreateShader(_tes, SHADER_TES);
	//	tcsHandle = CreateShader(_tcs, SHADER_TCS);
	//	if (tesHandle == -1 || tcsHandle == -1)
	//	{
	//		DELETESHADER(tesHandle);
	//		DELETESHADER(tcsHandle);
	//		return false;
	//	}
	//}

	//GLuint gsHandle = -1;
	//if (_gs != nullptr)
	//{
	//	gsHandle = CreateShader(_gs, SHADER_GEOMETRY);
	//	if (gsHandle == -1)
	//	{
	//		DELETESHADER(gsHandle);
	//		return false;
	//	}
	//}

	//GLuint csHandle = -1;
	//if (_cs != nullptr)
	//{
	//	csHandle = CreateShader(_cs, SHADER_COMPUTE);
	//	if (csHandle == -1)
	//	{
	//		if (csHandle != -1)
	//			glDeleteShader(csHandle);
	//		return false;
	//	}
	//}

	CREATESHADER(vsHandle, _vs, ShaderType::SHADER_VERTEX)
	CREATESHADER(fsHandle, _fs, ShaderType::SHADER_FRAGMENT)
	CREATESHADER(tesHandle, _tes, ShaderType::SHADER_TES)
	CREATESHADER(tcsHandle, _tcs, ShaderType::SHADER_TCS)
	CREATESHADER(gsHandle, _gs, ShaderType::SHADER_GEOMETRY)
	CREATESHADER(csHandle, _cs, ShaderType::SHADER_COMPUTE)

	bool ret = LinkProgram();

	DELETESHADER(vsHandle);
	DELETESHADER(fsHandle);
	DELETESHADER(tesHandle);
	DELETESHADER(tcsHandle);
	DELETESHADER(gsHandle);
	DELETESHADER(csHandle);

	return ret;
}

void Shader::AttachShader(GLuint shader)
{
	glAttachShader(_program, shader);
}

void Shader::DettachShader(GLuint shader)
{
	glDetachShader(_program, shader);
}

bool Shader::LinkProgram()
{
	GLint   status = 0;

	/// Á´½Ó³ÌÐò
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		char    compileLog[1024] = { 0 };
		glGetProgramInfoLog(_program, sizeof(compileLog), 0, compileLog);
		std::cout << compileLog << std::endl;
		return  false;
	}
	return  true;
}

unsigned Shader::CreateShader(const char* source, ShaderType type)
{
	unsigned    handle = 0;
	switch (type)
	{
	case ShaderType::SHADER_VERTEX:
		handle = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ShaderType::SHADER_GEOMETRY:
		handle = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case ShaderType::SHADER_FRAGMENT:
		handle = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case ShaderType::SHADER_TES:
		handle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;
	case ShaderType::SHADER_TCS:
		handle = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case ShaderType::SHADER_COMPUTE:
		handle = glCreateShader(GL_COMPUTE_SHADER);
		break;
	}

	GLint   status = 0;

	glShaderSource(handle, 1, (const GLchar**)&source, 0);
	glCompileShader(handle);
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		char    compileLog[1024] = { 0 };
		glGetShaderInfoLog(handle, sizeof(compileLog), 0, compileLog);
		std::cout << compileLog << std::endl;
		glDeleteShader(handle);
		return  -1;
	}

	AttachShader(handle);
	return  handle;
}
