#include "Player.h"
#include <iostream>
#include <Windows.h>
#include "glm\gtx\transform.hpp"
#include "glm\gtc\matrix_transform.hpp"

const float PI = 3.1415f;
const float halfPI = PI / 2;

Player::Player() {
	this->_position = { 0,0,3 };
	this->_lookat = { 0,0,-1 };
	this->_angleHor = 0.01f;
	this->_angleVer = 0.00f;
	this->_perspectiveMat = glm::perspective(45.0f, 1080.f / 720.0f, 1.0f, 50.0f);
	this->_viewMat = glm::lookAt(_position, _lookat, glm::vec3(0, 1, 0));
}

Player::~Player() {

}

void Player::update(float dt, SDL_Window &window) {
	
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
	GLuint camMatrix = glGetUniformLocation(shaderProgram, "Camera");
	GLuint perspMatrix = glGetUniformLocation(shaderProgram, "Perspective");
	
	_viewMat = glm::lookAt(_position, _position + _lookat, glm::vec3(0, 1, 0));

	glUniformMatrix4fv(perspMatrix, 1, GL_FALSE, &_perspectiveMat[0][0]);
	glUniformMatrix4fv(camMatrix, 1, GL_FALSE, &_viewMat[0][0]);
}

void Player::matrixUpdate2(const GLuint &shaderProgram) {
	GLuint camMatrix = glGetUniformLocation(shaderProgram, "cameraPos");
	GLuint perspMatrix = glGetUniformLocation(shaderProgram, "Perspective");

	_viewMat = glm::lookAt(_position, _position + _lookat, glm::vec3(0, 1, 0));
	glm::mat4x4 invPM = glm::inverse(_perspectiveMat);
	
	glUniformMatrix4fv(perspMatrix, 1, GL_FALSE, &invPM[0][0]);
	glUniform3fv(camMatrix, 1, &this->_position[0]);
}