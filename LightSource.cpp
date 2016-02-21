#include "LightSource.h"
#include <iostream>
#include <fstream>

LightSource::LightSource(const std::string &vertexPath, const std::string &fragmentPath): _shadowTex(0){
	_shadowShader.compileShaders(vertexPath, fragmentPath);
	_shadowShader.addAttribute("position");
	_shadowShader.linkShaders();
	initTex();
	initFrameBuffer();
}

LightSource::~LightSource() {

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
	glGenFramebuffers(1, &_fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _shadowTex, 0);
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "BALLAR OVERALLT!!!!";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


