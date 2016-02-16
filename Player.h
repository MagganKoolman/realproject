#pragma once
#include <SDL/SDL.h>
#include <gl\glew.h>
#include <glm\glm.hpp>
class Player {
public:
	Player();
	~Player();
	
	void update(float dt, SDL_Window &window);
	void matrixUpdate(const GLuint &shaderProgram);

private:
	glm::mat3x3 _rotY;
	glm::mat3x3 _rotX;
	glm::vec3 _position;
	glm::vec3 _lookat;
	float _angleHor;
	float _angleVer;

	glm::mat4 _perspectiveMat;
	glm::mat4 _viewMat;
	
	
};