#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <sstream>


class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader() noexcept;

	void use() const;

	void setBool(const std::string& name, bool val) const;
	void setInt(const std::string& name, int val) const;
	void setFloat(const std::string& name, float val) const;
	void setDouble(const std::string& name, double val) const;
	void setMat4(const std::string& name, const glm::mat4& val) const;
	void setVec3(const std::string& name, const glm::vec3& val) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
private:
	unsigned int ID;
};


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::stringstream vertexStream;
	std::stringstream fragmentStream;

	vertexFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fragmentFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();
	}
	catch (const std::ifstream::failure& err) {
		std::cout << "ERROR::SHADER::File_reading_error\n" << err.what() << std::endl;
	}

	std::string vertexContent = vertexStream.str();
	std::string fragmentContent = fragmentStream.str();
	const char* vertexCode = vertexContent.c_str();
	const char* fragmentCode = fragmentContent.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::Compilation_failed\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::Compilation_failed\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, 0, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::Linking_failed\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

inline Shader::~Shader() noexcept
{
	glDeleteProgram(ID);
}

inline void Shader::use() const
{
	glUseProgram(ID);
}

inline void Shader::setBool(const std::string& name, bool val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}

inline void Shader::setInt(const std::string& name, int val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

inline void Shader::setFloat(const std::string& name, float val) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

inline void Shader::setDouble(const std::string& name, double val) const
{
	glUniform1d(glGetUniformLocation(ID, name.c_str()), val);
}

inline void Shader::setMat4(const std::string& name, const glm::mat4& val) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

inline void Shader::setVec3(const std::string& name, const glm::vec3& val) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), val.x, val.y, val.z);
}

inline void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

#endif