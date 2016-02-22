#include "App.h"
#include <cstddef>
#include "Player.h"
#include "LightSource.h"

struct ScreenVertex {
	float x, y, s, t;
};

App::App()
{
	this->window = nullptr;
	this->screen_height = 720;
	this->screen_width = 1080;
	this->_player = Player();
	init();
}

App::~App()
{
	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}
	this->models.clear();
}

void App::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Try hard!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL);

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
		std::cout << "SDLFEL";

	GLenum error = glewInit();
	if (error != GLEW_OK)
		std::cout << "GlewFel!";

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);

	initShader();
	glEnable(GL_DEPTH_TEST);
	//Börjar fucka runt!!!
	glGenBuffers(1, &screen);
	ScreenVertex vertexData[6];

	//first triangle
	vertexData[0].x = -1.0;
	vertexData[0].y = -1.0;
	vertexData[0].s = 0.0;
	vertexData[0].t = 0.0;

	vertexData[1].x = -1.0;
	vertexData[1].y = 1.0;
	vertexData[1].s = 0.0;
	vertexData[1].t = 1.0;

	vertexData[2].x = 1.0;
	vertexData[2].y = -1.0;
	vertexData[2].s = 1.0;
	vertexData[2].t = 0.0;

	vertexData[3].x = 1.0;
	vertexData[3].y = -1.0;
	vertexData[3].s = 1.0;
	vertexData[3].t = 0.0;

	vertexData[4].x = 1.0;
	vertexData[4].y = 1.0;
	vertexData[4].s = 1.0;
	vertexData[4].t = 1.0;

	vertexData[5].x = -1.0;
	vertexData[5].y = 1.0;
	vertexData[5].s = 0.0;
	vertexData[5].t = 1.0;

	importer = new OBJimporter();

	importer->loadObj("models/sphere1.obj");
	models = importer->CreateTriangleData();
	

	delete importer;
	
	importer = new OBJimporter();

	std::vector<Model*> temp;
	importer->loadObj("models/box.obj");
	temp = importer->CreateTriangleData();
	for (int i = 0; i < temp.size(); i++) {
		models.push_back(temp[i]);
	}

	delete importer;
	
	glBindBuffer(GL_ARRAY_BUFFER, screen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void App::initShader() {
	/*testProgram.compileShaders("shaders/testVertex.vert", "shaders/testFragment.frag");
	testProgram.addAttribute("vertexPos");
	testProgram.addAttribute("texCoorIn");
	testProgram.linkShaders();
	*/
	_colorProgram.compileShaders("shaders/ColorShader.vert", "shaders/ColorShader.frag");
	_colorProgram.addAttribute("position");
	_colorProgram.addAttribute("texturePos");
	_colorProgram.linkShaders();

	//LightSource lights("shaders/ColorShader.vert", "shaders/ColorShader.frag");

	_deferredProgram.compileShaders("shaders/DeferredVertex.vert", "shaders/DeferredFragment.frag");
	_deferredProgram.addAttribute("vertexPos");
	_deferredProgram.addAttribute("normal");
	_deferredProgram.addAttribute("texCoorIn");
	_deferredProgram.initFrameBuffer();
	_deferredProgram.linkShaders();
}


void App::update(){
	float x = 0.01f;
	_player.update(x, *window);
	render();
}

void App::render() {
	/*testProgram.use();
	_player.matrixUpdate(testProgram.getProgramID());
	for (int i = 0; i < models.size(); i++) {
		models[i]->draw();
	}
	testProgram.unUse();*/
	
	_deferredProgram.use();
	_player.matrixUpdate(_deferredProgram.getProgramID());
	
	for (int i = 0; i < models.size(); i++) {
		models[i]->draw(_deferredProgram.getProgramID());
	}
	
	_deferredProgram.unUse();
	_colorProgram.use();		
	_colorProgram.enableTextures(_deferredProgram);

	_player.matrixUpdate2(_colorProgram.getProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, screen);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), (void*)offsetof(ScreenVertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), (void*)offsetof(ScreenVertex, s));	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	_colorProgram.disableTextures();
	_colorProgram.unUse();
	
	SDL_GL_SwapWindow(window);
}

