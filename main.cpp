#include "App.h"
#include <GL\glew.h>
#include <iostream>
#include <chrono>

using namespace std;
int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	App app;

	chrono::milliseconds timeStamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
	chrono::milliseconds timeStamp2;
	long long dt = timeStamp.count();
	bool running = true;
	
	while (running)
	{
		timeStamp2 = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
		dt = timeStamp2.count() - timeStamp.count();
		timeStamp = timeStamp2;
		running = !GetAsyncKeyState(VK_ESCAPE);
		app.update(dt/1000.f);
	}

	return 0;
}

