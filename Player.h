#pragma once
#include <SDL/SDL.h>
#include <gl\glew.h>
#include <glm\glm.hpp>
#include "Datastructs.h"
class Player {
public:
	Player();
	~Player();
	
	void update(float dt, SDL_Window &window);
	void matrixUpdate(const GLuint &shaderProgram);
	void matrixUpdate2(const GLuint &shaderProgram);
	void setHM(unsigned char* hm);
	void updateFrustrum();
	FrustumPoints getFrustum() const;
	glm::vec3 _lookat;
private:
	glm::mat3x3 _rotY;
	glm::mat3x3 _rotX;
	
	float _angleHor;
	float _angleVer;
	glm::vec3 position;
	FrustumPoints frustum;

	glm::mat4 _perspectiveMat;
	glm::mat4 _viewMat;
	
	unsigned char* heightMap;
};