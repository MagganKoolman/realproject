#include "Compute.h"
#include <iostream>
#include <fstream>
#include <vector>

Compute::Compute(): _computeShader(0), _programID(0) {

}

Compute::~Compute() {
	
}

void Compute::initComputeShader(const std::string &filePath) {	
	_programID = glCreateProgram();
	_computeShader = glCreateShader(GL_COMPUTE_SHADER);
	if (_computeShader == 0) {
		std::cout << "Compute shader fucka!";
	}
	compileComputeShader(filePath, _computeShader);
	glAttachShader(_programID, _computeShader);
	glLinkProgram(_programID);
	GLint success = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);
		glDeleteProgram(_programID);
		glDeleteShader(_computeShader);
		std::cout << "Compute shader fucka \n";
		std::printf("%s\n", &(errorLog[0]));
	}
	glDetachShader(_programID, _computeShader);
	glDeleteShader(_computeShader);
}

void Compute::compileComputeShader(const std::string &filePath, GLuint shaderID) {
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		std::cout << "Fil fucka: " + filePath;
	}
	std::string fileContent = "";
	std::string line;

	while (std::getline(shaderFile, line)) {
		fileContent += line + "\n";
	}
	shaderFile.close();

	const char* contentsPtr = fileContent.c_str();
	glShaderSource(shaderID, 1, &contentsPtr, nullptr);
	glCompileShader(shaderID);

	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(shaderID);
		std::printf("%s\n", &(errorLog[0]));
	}
}

GLuint Compute::getProgramID() const {
	return _programID;
}