#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/* Vertex and Fragment shaders, program usage class */
class Shader
{
public:
	/* Construction of shaders and the program */
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader() { glDeleteProgram(ID); }
	/* Use the program */
	void use();
	/* Utility uniform functions */
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	unsigned int ID;
};


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vertexFile, fragmentFile;
	std::stringstream vertexStream, fragmentStream;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();
		
		vertexFile.close();
		fragmentFile.close();
	}

	catch (std::ifstream::failure err) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << err.what() << std::endl;
	}

	std::string vertexCode(std::move(vertexStream.str())), fragmentCode(std::move(fragmentStream.str()));

	const char* vShaderSource = vertexCode.c_str(),
		* fShaderSource = fragmentCode.c_str();

	int success;
	char infoLog[512];
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderSource, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderSource, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

inline void Shader::use()
{
	glUseProgram(ID);
}

inline void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

inline void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


inline void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

#endif