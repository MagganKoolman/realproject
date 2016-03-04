#pragma once
#include "GLSLprogram.h"
#include <glm\glm.hpp>
class LightSource{
public:
	LightSource();
	~LightSource();

	void updateShadows();
	void init(const std::string& vertexPath, const std::string& fragmentPath);
	void activateShadowMap(const GLuint &shaderProgram);
	void deActivateShadowMap(const GLuint &shaderProgram);
	
	void updatePosition(float dt);

	GLSLprogram shadowShader;
	GLuint _shadowTex;
	
private:
	GLuint _fboID;
	
	glm::mat4 bias_matrix;
	glm::mat4 _perspectiveMat;
	glm::mat4 _viewMat;

	void initTex();
	void initFrameBuffer();

	struct LightProperties {
		glm::vec3 position;
		glm::vec3 direction;
	}light;
};