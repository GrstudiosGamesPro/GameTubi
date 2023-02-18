#include "box2d.h"
#include <iostream>
#include "Source/Engine/Window.h"
#include "Source/Engine/Components/SceneManager/ManagerScene.h"
#include "Source/Engine/Components/InputSystem/InputSystem.h"


using namespace std;

int main (int argc, char *argv[])
{
	InputSystem::create();
	ManagerScene::create();

	const int FPS = 144;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	Window* CreateWindowsEngine = new Window();

	CreateWindowsEngine->StartWindow ("GameTubi Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1900, 1000, false);


	while (CreateWindowsEngine->running()) {
		frameStart = SDL_GetTicks();
		CreateWindowsEngine->OnUpdate();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay (frameDelay - frameTime);
		}
	}

	InputSystem::release();
	ManagerScene::release();
	CreateWindowsEngine->OnClean();
	return 0;
}