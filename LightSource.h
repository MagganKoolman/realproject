#pragma once
#include "GLSLprogram.h"
#include <glm\glm.hpp>
class LightSource{
public:
	LightSource();
	~LightSource();

	void updateShadows();
	void init(const std::string& vertexPath, const std::string& fragmentPath);
	
	GLSLprogram shadowShader;
private:
	GLuint _shadowTex;	
	GLuint _fboID;
	
	glm::mat4 _perspectiveMat;
	glm::mat4 _viewMat;

	void initTex();
	void initFrameBuffer();

	struct LightProperties {
		glm::vec3 position;
		glm::vec3 direction;
	}light;
};