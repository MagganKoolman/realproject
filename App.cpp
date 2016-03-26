#include "App.h"
#include <cstddef>
#include <algorithm> 

struct ScreenVertex {
	float x, y, s, t;
};

App::App()
{
	this->window = nullptr;
	this->screen_height = 720;
	this->screen_width = 1080;
	init();
}

App::~App()
{
	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}
	this->models.clear();
	delete quadTree;
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
	initGaussTex();
	
	importer = new OBJimporter();

	importer->loadObj("models/sphere1.obj");
	models = importer->CreateTriangleData();
	models[0]->createBBox("sphere.txt");
	models[0]->addTranslation(vec3(5, 2, -5));
	models[0]->initNormalTexture("normalMap.png");
	delete importer;

	importer = new OBJimporter();
	std::vector<Model*> temp;

	importer->loadObj("models/box.obj");
	temp = importer->CreateTriangleData();
	temp[0]->createBBox("box.txt");
	
	Model* box2;
	box2 = new Model(*temp[0]);
	Model* box3;
	box3 = new Model(*temp[0]);
	
	temp[0]->addTranslation(vec3(3,0,3));
	box2->addTranslation(vec3(-18, 10, -18));
	box3->addTranslation(vec3(10, 3, -3));
	models.push_back(temp[0]);
	models.push_back(box2);
	models.push_back(box3);
		
	unsigned char* Pheightmap = nullptr;
	Model* hm = importer->getGround("height_map2.bmp", Pheightmap);
	hm->createBBox("terrain.txt");
	models.push_back(hm);
	_player.setHM(Pheightmap);
	delete importer;
	createScreenQuad();

	quadTree = new QuadTree(vec3(-25, 0, 25), 50);
	quadTree->buildTree(models);
}

void App::initShader() {
	testProgram.compileShaders("shaders/testVertex.vert", "shaders/testFragment.frag", " ");
	testProgram.addAttribute("position");	
	testProgram.addAttribute("texturePos");
	testProgram.linkShaders();
	
	_colorProgram.compileShaders("shaders/ColorShader.vert", "shaders/ColorShader.frag", " ");
	_colorProgram.addAttribute("position");
	_colorProgram.addAttribute("texturePos");
	_colorProgram.linkShaders();
	
	_deferredProgram.compileShaders("shaders/DeferredVertex.vert", "shaders/DeferredFragment.frag", "shaders/DeferredGeometry.geo");
	_deferredProgram.addAttribute("vertexPos");
	_deferredProgram.addAttribute("normal");
	_deferredProgram.addAttribute("texCoorIn");
	_deferredProgram.initFrameBuffer();
	_deferredProgram.linkShaders();

	_wireFrameProgram.compileShaders("shaders/WireframeShader.vert", "shaders/WireframeShader.frag", " ");
	_wireFrameProgram.addAttribute("vertexPos");
	_wireFrameProgram.linkShaders();

	lights.init("shaders/ShadowVertex.vert", "shaders/ShadowFragment.frag");
	gaussianFilter.initComputeShader("shaders/GaussianFilter.comp");
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

void App::update(float deltaTime){
	_player.update(deltaTime, *window);
	this->lights.updatePosition(deltaTime);
	render();
}

void App::deferredDraw() {
	_deferredProgram.use();
	_player.matrixUpdate(_deferredProgram.getProgramID());

	std::vector<Model*> modelsToBeDrawn = quadTree->checkIntersection(_player.getFrustum());
	std::sort(modelsToBeDrawn.begin(), modelsToBeDrawn.begin() + modelsToBeDrawn.size());
	Model* lastModelDrawn = nullptr;
	int x = 0;
	for (int i = 0; i < modelsToBeDrawn.size(); i++)
	{
		if (lastModelDrawn != modelsToBeDrawn[i])
		{
			modelsToBeDrawn[i]->draw(_deferredProgram.getProgramID());
			x++;
		}
		lastModelDrawn = modelsToBeDrawn[i];
	}
	std::cout << x << std::endl;

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 4.0f);
	_deferredProgram.unUse();

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
		deferredDraw();
		if (GetAsyncKeyState('E'))
		{
			gaussiate();
			_colorProgram.use();
			_colorProgram.enableTextures(_deferredProgram);
			GLuint texLocation = glGetUniformLocation(_colorProgram.getProgramID(), "colorTex");
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gaussianTexture);
			glUniform1i(texLocation, 0);
		}
		else
		{
			_colorProgram.use();
			_colorProgram.enableTextures(_deferredProgram);
		}
		_player.matrixUpdate2(_colorProgram.getProgramID());
		lights.activateShadowMap(_colorProgram.getProgramID());
	
		drawOnScreenQuad();
		
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
		drawOnScreenQuad();
	}	
	SDL_GL_SwapWindow(window);
}

void App::drawOnScreenQuad() {
	glBindBuffer(GL_ARRAY_BUFFER, screen);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), (void*)offsetof(ScreenVertex, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertex), (void*)offsetof(ScreenVertex, s));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void App::gaussiate() {
	glUseProgram(gaussianFilter.getProgramID());
	glBindImageTexture(0, _deferredProgram.getTexture(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, gaussianTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(135, 90, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
 	glUseProgram(0);
}


void App::initGaussTex() {
	glGenTextures(1, &gaussianTexture);
	glBindTexture(GL_TEXTURE_2D, gaussianTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1080, 720, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}


