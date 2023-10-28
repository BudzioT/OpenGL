#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader
{
public:
	/* Constructor and destructor */
	Shader(const std::string& pathVertex, const std::string& pathFragment);
	~Shader() noexcept;
	/* Use function */
	void use() const noexcept;
	/* Set functions */
	void setInt(const std::string& name, int val) const;
	void setBool(const std::string& name, bool val) const;
	void setFloat(const std::string& name, float val) const;
	void setDouble(const std::string& name, double val) const;
	void setMat4f(const std::string& name, glm::mat4 val) const;
private:
	unsigned int ID;
};


Shader::Shader(const std::string& pathVertex, const std::string& pathFragment)
{
	std::ifstream fileVertex;
	std::ifstream fileFragment;
	std::stringstream streamVertex;
	std::stringstream streamFragment;

	fileVertex.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fileFragment.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		fileVertex.open(pathVertex);
		fileFragment.open(pathFragment);

		streamVertex << fileVertex.rdbuf();
		streamFragment << fileFragment.rdbuf();

		fileVertex.close();
		fileFragment.close();
	}

	catch (std::ifstream::failure fail) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << fail.what() << std::endl;
	}

	std::string sCodeVertex(streamVertex.str());
	std::string sCodeFragment(streamFragment.str());
	const char* codeVertex = sCodeVertex.c_str();
	const char* codeFragment = sCodeFragment.c_str();

	int success;
	char infoLog[512];

	unsigned int shaderVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderVertex, 1, &codeVertex, NULL);
	glCompileShader(shaderVertex);
	glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderVertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderFragment, 1, &codeFragment, NULL);
	glCompileShader(shaderFragment);
	glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderFragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, shaderVertex);
	glAttachShader(ID, shaderFragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(shaderFragment);
	glDeleteShader(shaderVertex);
}

inline void Shader::use() const noexcept
{
	glUseProgram(ID);
}

inline Shader::~Shader() noexcept
{
	glDeleteProgram(ID);
}

inline void Shader::setInt(const std::string& name, int val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

inline void Shader::setBool(const std::string& name, bool val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}

inline void Shader::setFloat(const std::string& name, float val) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

inline void Shader::setDouble(const std::string& name, double val) const
{
	glUniform1d(glGetUniformLocation(ID, name.c_str()), val);
}

inline void Shader::setMat4f(const std::string& name, glm::mat4 val) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

#endif