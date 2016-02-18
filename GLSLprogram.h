#pragma once
#include <string>
#include <GL/glew.h>
#include <iostream>

class GLSLprogram {
public:
	GLSLprogram();
	~GLSLprogram();
	
	void compileShaders(const std::string& vertexPath, const std::string& fragmentPath);
	void linkShaders();
	void addAttribute(const std::string& attributeName);
	void initFrameBuffer();

	void use();
	void unUse();

	void enableTextures(const GLSLprogram &secondShader);
	void disableTextures();

	GLuint getTexture() const;
	GLuint getTexture2() const;
	GLuint getProgramID() const;
private:
	GLuint _programID;
	GLuint _vertexShader;
	GLuint _fragmentShader;
	
	GLuint _frameBuffer;
	GLuint _texture;
	GLuint _normalTexture;
	GLuint _depthRenderBuffer;

	int _numAttributes;
	void compileShader(const std::string& filePath, GLuint shaderID);
	
};