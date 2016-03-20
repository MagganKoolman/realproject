#include "Player.h"
#include <iostream>
#include <Windows.h>
#include "glm\gtx\transform.hpp"
#include "glm\gtc\matrix_transform.hpp"

const float PI = 3.1415f;
const float halfPI = PI / 2;

Player::Player() {
	this->_position = { 0,0,0 };
	this->_lookat = { -1,0, 0 };
	this->_angleHor = 0.01f;
	this->_angleVer = 0.00f;
	this->_perspectiveMat = glm::perspective(45.0f, 1080.f / 720.0f, 0.5f, 50.0f);
	this->_viewMat = glm::lookAt(_position, _lookat, glm::vec3(0, 1, 0));
}

Player::~Player() {
	delete[] this->heightMap;
}

void Player::update(float dt, SDL_Window &window) {
	dt = dt*10;
	int index = ((int)_position.z + 20) * 200 + (int)_position.x+20;
	_position.y = heightMap[index*5]/25.0f +0.5f;
	if (GetAsyncKeyState('W')) {
		_position = _position + glm::vec3(_lookat.x, 0, _lookat.z)*dt;
	}
	if (GetAsyncKeyState('S')) {
		_position = _position - glm::vec3(_lookat.x, 0, _lookat.z)*dt;
	}
	if (GetAsyncKeyState('A')) {
		glm::vec3 rightVec = glm::cross(glm::vec3(0, 1, 0), _lookat);
		_position = _position + rightVec*dt;
	}
	if (GetAsyncKeyState('D')) {
		glm::vec3 rightVec = glm::cross(glm::vec3(0, 1, 0), _lookat);
		_position = _position - rightVec*dt;
	}
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {		
		if (SDL_MOUSEMOTION == evnt.type) {
			_angleHor = (520 - evnt.motion.x) * -0.001f;
			if (std::abs(_angleHor) < 0.002f)
				_angleHor = 0.0f;
			
			_angleVer = (360 - evnt.motion.y) * -0.001f;
			if (std::abs(_angleVer) < 0.002f)
				_angleVer = 0.0f;
			if (_lookat.y > 2.00f)
				_lookat.y = 1.995f;
			else if (_lookat.y < -2.00f)
				_lookat.y = -1.995f;
		}
	}
	SDL_WarpMouseInWindow(&window, 520, 360);
	_rotX = { glm::vec3(cosf(_angleHor), 0, sinf(_angleHor)), glm::vec3(0,1,0), glm::vec3(-sinf(_angleHor), 0, cosf(_angleHor)) };
	_lookat = _rotX*_lookat;
	_lookat.y -= _angleVer;
}

void Player::matrixUpdate(const GLuint &shaderProgram) {
	_viewMat = glm::lookAt(_position, _position + _lookat, glm::vec3(0, 1, 0));
	GLuint camMatrix = glGetUniformLocation(shaderProgram, "Camera");
	glUniformMatrix4fv(camMatrix, 1, GL_FALSE, &_viewMat[0][0]);
	
	GLuint perspMatrix = glGetUniformLocation(shaderProgram, "Perspective");
	glUniformMatrix4fv(perspMatrix, 1, GL_FALSE, &_perspectiveMat[0][0]);
	
	GLuint camPos = glGetUniformLocation(shaderProgram, "cameraPosition");	
	glUniform3fv(camPos, 1, &_position[0]);
}

void Player::matrixUpdate2(const GLuint &shaderProgram) {
	GLuint camPos = glGetUniformLocation(shaderProgram, "cameraPos");
	GLuint perspMatrix = glGetUniformLocation(shaderProgram, "invPerspective");

	glm::mat4x4 invPM = glm::inverse(_perspectiveMat * _viewMat);
	glm::vec3 viewDir = glm::normalize(_lookat - _position);
	glUniformMatrix4fv(perspMatrix, 1, GL_FALSE, &invPM[0][0]);
	glUniform3fv(camPos, 1, &_position[0]);
}

void Player::setHM(unsigned char* hm) {
	this->heightMap = hm;
}