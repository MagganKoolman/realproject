#include "LightSource.h"
#include <iostream>
#include "glm\gtx\transform.hpp"
#include "glm\gtc\matrix_transform.hpp"

LightSource::LightSource(): _shadowTex(0){
	
	light.position = glm::vec3(5, 8, 0);
	light.direction = glm::vec3(0.01f, 0, 0.01f);

	_perspectiveMat = glm::perspective(45.0f, 1080.f / 720.0f, 3.0f, 20.0f);
	_viewMat = glm::lookAt(light.position, light.direction, glm::vec3(0, 1, 0));

	bias_matrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
		) * _perspectiveMat * _viewMat;
}

LightSource::~LightSource() {

}

void LightSource::init(const std::string& vertexPath, const std::string& fragmentPath) {
	shadowShader.compileShaders(vertexPath, fragmentPath, " ");
	shadowShader.addAttribute("vertexPosition");
	shadowShader.linkShaders();
	initTex();
	initFrameBuffer();
}

void LightSource::initTex() {
	glGenTextures(1, &_shadowTex);
	glBindTexture(GL_TEXTURE_2D, _shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 1080, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LightSource::initFrameBuffer() {
	glGenFramebuffers(1, &shadowShader.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowShader.frameBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _shadowTex, 0);
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Shadow FBO buggar!!!";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightSource::updateShadows() {
	GLuint VPmatrix = glGetUniformLocation(shadowShader.getProgramID(), "VPmatrix");
	glUniformMatrix4fv(VPmatrix, 1, GL_FALSE, &bias_matrix[0][0]);
}

void LightSource::activateShadowMap(const GLuint &shaderProgram)
{
	GLuint WPmatrix = glGetUniformLocation(shaderProgram, "LightWP");
	glUniformMatrix4fv(WPmatrix, 1, GL_FALSE, &bias_matrix[0][0]);

	GLuint lightPosition = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3fv(lightPosition, 1, &light.position[0]);

	GLuint texLocation = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(texLocation, 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, _shadowTex);
}

void LightSource::deActivateShadowMap(const GLuint &shaderProgram) {
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);
}



