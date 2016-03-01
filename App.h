#pragma once

#include <vector>
#include "Datastructs.h"
#include "Model.h"
#include "OBJimporter.h"
#include "LightSource.h"

#include <iostream>
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <Windows.h>
#include "Player.h"
#include "GLSLprogram.h"


class App {
private:
	SDL_Window* window;
	
	Player _player;
	int screen_height;
	int screen_width;

	void init();
	void initShader();
	void render();
	void createScreenQuad();

	std::vector<Model*> models;
	OBJimporter* importer;

	GLuint screen;

	LightSource lights;

	GLSLprogram _colorProgram;
	GLSLprogram _deferredProgram;
	
	GLSLprogram testProgram;

	GLSLprogram _wireFrameProgram;
public:
	App();
	~App();
	
	void update(float deltaTime);
};
