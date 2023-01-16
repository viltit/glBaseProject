#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <vector>

namespace moe {

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
    :   _id         { glCreateProgram() },
        _vertexID   { glCreateShader(GL_VERTEX_SHADER) },
        _fragmentID { glCreateShader(GL_FRAGMENT_SHADER) },
        _geoID      { 0 },
        _tessID     { 0 }
{
    //TODO: Geo and Tesselation shaders
    if (_id == 0 || _vertexID == 0 || _fragmentID == 0) {
        //TODO: Proper error handling
        std::cout << "Failed to initialize shader." << std::endl;
        return;
    }

    compile(vertexShader, _vertexID);
    compile(fragmentShader, _fragmentID);
    link();

}

Shader::~Shader() {
    if (_id != 0)           glDeleteProgram(_id);
    if (_vertexID != 0)     glDeleteShader(_vertexID);
    if (_fragmentID != 0)   glDeleteShader(_fragmentID);
    if (_geoID != 0)        glDeleteShader(_geoID);
    if (_tessID != 0)       glDeleteShader(_tessID);
}

void Shader::on() const {
    glUseProgram(_id);
}
void Shader::off() const {
    glUseProgram(0);
}

GLint Shader::uniform(const std::string & name) const {
	GLint location = glGetUniformLocation(_id, name.c_str());
	if (location == GL_INVALID_INDEX) {
        //TODO: Proper error handling
		std::string message = "Uniform " + name + " not found";
		std::cout << message << std::endl;
	}
	return location;
}

void Shader::compile(const std::string& shader, GLuint shaderID) {

    const char* contentPtr = shader.c_str();
    glShaderSource(shaderID, 1, &contentPtr, nullptr);
    glCompileShader(shaderID);

    GLint status { 0 };
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (!status) {
        GLint length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetShaderInfoLog(shaderID, length, &length, &(log[0]));
		glDeleteShader(shaderID);
		
        //TODO: Proper error handling
        std::cout << "Unable to compile shader " << shader << " Error-Message:" << std::endl;
        std::cout << log.data();
    }
}

void Shader::link() {

    //Attach Shaders to the program:
	glAttachShader(_id, _vertexID);
	glAttachShader(_id, _fragmentID);
	if (_geoID != 0) glAttachShader(_id, _geoID); //it WOULD be possible to attach an empty shader...
	if (_tessID != 0) glAttachShader(_id, _tessID);

	//link the Program:
	glLinkProgram(_id);

	//Check for success:
	GLint success{ 0 };
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success) {
		GLint length;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetProgramInfoLog(_id, length, &length, &(log[0]));

        //TODO: Proper error handling
        std::cout << "Unable to link shaders. Error-Message:" << std::endl;
        std::cout << log.data();
	}
}
}