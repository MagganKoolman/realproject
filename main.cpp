#include "App.h"
#include <GL\glew.h>
#include <iostream>


int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	App app;

	bool running = true;
	while (running)
	{
		running = !GetAsyncKeyState(VK_ESCAPE);
		app.update();
	}

	return 0;
}

