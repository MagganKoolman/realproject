#pragma once
#include "GLSLprogram.h"
#include <glm\glm.hpp>
class LightSource{
public:
	LightSource(const std::string& vertexPath, const std::string& fragmentPath);
	~LightSource();
	struct LightProperties {
		glm::vec3 position;
	}light;
	
private:
	GLuint _shadowTex;	
	GLuint _fboID;
	GLSLprogram _shadowShader;

	void initTex();
	void initFrameBuffer();
};