#pragma once

#include <vector>
#include "Datastructs.h"
#include "Model.h"
#include "OBJimporter.h"

#include <iostream>
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <Windows.h>
#include "Player.h"
#include "GLSLprogram.h"

#include "Sprite.h"

class App {
private:
	SDL_Window* window;
	
	Player _player;
	int screen_height;
	int screen_width;

	void init();
	void initShader();
	void render();

	Sprite testSprite;
	Sprite testSprite2;
	Sprite testSprite3;

	std::vector<Model*> models;
	OBJimporter* importer;

	GLuint screen;

	GLSLprogram _colorProgram;
	GLSLprogram _deferredProgram;
	
	GLSLprogram testProgram;
public:
	App();
	~App();
	
	void update();
};
