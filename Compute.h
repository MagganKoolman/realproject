#pragma once
#include <GL\glew.h>
#include <string>

class Compute {
public:
	Compute();
	~Compute();
	
	void initComputeShader(const std::string &filePath);
	GLuint getProgramID()const;
private:
	GLuint _computeShader;
	GLuint _programID;

	void compileComputeShader(const std::string &filePath, GLuint shaderID);
	void linkShader();
};