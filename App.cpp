#include "App.h"
#include <cstddef>

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
	glEnable(GL_DEPTH_TEST);

	initShader();

	importer = new OBJimporter();

	importer->loadObj("models/sphere1.obj");
	models = importer->CreateTriangleData();
	models[0]->addTranslation(vec3(0,0.5,0));
	delete importer;

	//skapa 2dra bollen

	Model* boll2 = new Model();
	Material* tempMat = models[0]->getMat();
	boll2->setMaterial(tempMat);
	boll2->setBUFFid(models[0]->getBuffID());
	boll2->setSize(models[0]->getSize());
	boll2->addTranslation(vec3(4,0.5,0));
	models.push_back(boll2);

	//3dje:

	Model* boll3 = new Model();
	boll3->setMaterial(tempMat);
	boll3->setBUFFid(models[0]->getBuffID());
	boll3->setSize(models[0]->getSize());
	boll3->addTranslation(vec3(0, 0.5, 3));
	models.push_back(boll3);
	//4de:
	Model* boll4 = new Model();
	boll4->setMaterial(tempMat);
	boll4->setBUFFid(models[0]->getBuffID());
	boll4->setSize(models[0]->getSize());
	boll4->addTranslation(vec3(3, 0.5, 3));
	models.push_back(boll4);

	importer = new OBJimporter();
	std::vector<Model*> temp;

	importer->loadObj("models/box.obj");
	temp = importer->CreateTriangleData();
	for (int i = 0; i < temp.size(); i++) {
		temp[i]->addTranslation(vec3(i+2,0,0));
		models.push_back(temp[i]);

	}
	Model* hm = importer->getGround("height_map2.bmp");
	models.push_back(hm);
	delete importer;
	createScreenQuad();
}

void App::initShader() {
	/*testProgram.compileShaders("shaders/testVertex.vert", "shaders/testFragment.frag");
	testProgram.addAttribute("vertexPos");
	testProgram.addAttribute("texCoorIn");
	testProgram.linkShaders();*/
	
	_colorProgram.compileShaders("shaders/ColorShader.vert", "shaders/ColorShader.frag", " ");
	_colorProgram.addAttribute("position");
	_colorProgram.addAttribute("texturePos");
	_colorProgram.linkShaders();

	lights.init("shaders/ShadowVertex.vert", "shaders/ShadowFragment.frag");

	_deferredProgram.compileShaders("shaders/DeferredVertex.vert", "shaders/DeferredFragment.frag", "shaders/DeferredGeometry.geo");
	_deferredProgram.addAttribute("vertexPos");
	_deferredProgram.addAttribute("normal");
	_deferredProgram.addAttribute("texCoorIn");
	_deferredProgram.initFrameBuffer();
	_deferredProgram.linkShaders();

	_wireFrameProgram.compileShaders("shaders/WireframeShader.vert", "shaders/WireframeShader.frag", " ");
	_wireFrameProgram.addAttribute("vertexPos");
	_wireFrameProgram.linkShaders();
}

void App::createScreenQuad() {
	glGenBuffers(1, &screen);
	ScreenVertex vertexData[6];
	vertexData[0].x = -1.0; vertexData[0].y = -1.0; vertexData[0].s = 0.0; vertexData[0].t = 0.0;
	vertexData[1].x = -1.0; vertexData[1].y = 1.0; vertexData[1].s = 0.0; vertexData[1].t = 1.0;
	vertexData[2].x = 1.0; vertexData[2].y = -1.0; vertexData[2].s = 1.0; vertexData[2].t = 0.0;
	vertexData[3].x = 1.0; vertexData[3].y = -1.0; vertexData[3].s = 1.0; vertexData[3].t = 0.0;
	vertexData[4].x = 1.0; vertexData[4].y = 1.0; vertexData[4].s = 1.0; vertexData[4].t = 1.0;
	vertexData[5].x = -1.0; vertexData[5].y = 1.0; vertexData[5].s = 0.0; vertexData[5].t = 1.0;
	glBindBuffer(GL_ARRAY_BUFFER, screen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void App::update(){
	float x = 0.1f;
	_player.update(x, *window);
	render();
}

void App::render() {
	
	lights.shadowShader.use();
	lights.updateShadows();
	for (int i = 0; i < models.size(); i++) {
		models[i]->draw(lights.shadowShader.getProgramID());
	}
	
	lights.shadowShader.unUse();
	
	if (!GetAsyncKeyState('Q'))
	{
		_deferredProgram.use();
		_player.matrixUpdate(_deferredProgram.getProgramID());
		for (int i = 0; i < models.size(); i++) {
			models[i]->draw(_deferredProgram.getProgramID());
		}
		
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.0f, 4.0f);
		_deferredProgram.unUse();
		_colorProgram.use();		
		_colorProgram.enableTextures(_deferredProgram);
		_player.matrixUpdate2(_colorProgram.getProgramID());
		lights.activateShadowMap(_colorProgram.getProgramID());
		glBindBuffer(GL_ARRAY_BUFFER, screen);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), (void*)offsetof(ScreenVertex, x));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), (void*)offsetof(ScreenVertex, s));	
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		_colorProgram.disableTextures();
		lights.deActivateShadowMap(_colorProgram.getProgramID());
		_colorProgram.unUse();
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		_wireFrameProgram.use();
		_player.matrixUpdate(_wireFrameProgram.getProgramID());
		for (int i = 0; i < models.size(); i++) {
			models[i]->draw(_wireFrameProgram.getProgramID());
		}
		_wireFrameProgram.unUse();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	SDL_GL_SwapWindow(window);
}

