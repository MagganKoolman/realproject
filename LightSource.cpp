#include "LightSource.h"
#include <iostream>
#include "glm\gtx\transform.hpp"
#include "glm\gtc\matrix_transform.hpp"

LightSource::LightSource(): _shadowTex(0){
	
	light.position = glm::vec3(0, 3, -5);
	light.direction = glm::vec3(0, 0, 0);

	_perspectiveMat = glm::perspective(45.0f, 1080.f / 720.0f, 0.0f, 20.0f);
	_viewMat = glm::lookAt(light.position, light.direction, glm::vec3(0, 1, 0));
}

LightSource::~LightSource() {

}

void LightSource::init(const std::string& vertexPath, const std::string& fragmentPath) {
	shadowShader.compileShaders(vertexPath, fragmentPath);
	shadowShader.addAttribute("position");
	shadowShader.linkShaders();
	initTex();
	initFrameBuffer();
}

void LightSource::initTex() {
	glGenTextures(1, &_shadowTex);
	glBindTexture(GL_TEXTURE_2D, _shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 1080, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
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
	_viewMat = glm::lookAt(light.position, light.direction, glm::vec3(0, 1, 0));
	glm::mat4x4 ViewPerspectiveMat = _perspectiveMat * _viewMat;
	GLuint VPmatrix = glGetUniformLocation(shadowShader.getProgramID(), "VPmatrix");
	glUniformMatrix4fv(VPmatrix, 1, GL_FALSE, &ViewPerspectiveMat[0][0]);
}


