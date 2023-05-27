#include "box2d.h"
#include "Source/Engine/Window.h"
#include "Source/Engine/Components/SceneManager/ManagerScene.h"
#include "Source/Engine/Components/InputSystem/InputSystem.h"
#include "Source/Engine/Components/UIBuilder/UIBuilder.h"
#include "Source/Engine/Components/SaveSystem/SaveData.h"
#include "Source/Engine/Components/Console/ConsoleManager.h"
#include "Source/Engine/Components/EditorPlay/PlayMode.h"
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{
	InputSystem::create			();
	ManagerScene::create		();
	UIBuilder::create			();
	SaveData::create			();
	ConsoleManager::create		();
	PlayMode::create			();

	SaveData::GetInstance()->LoadEngineData();

	const int FPS = 144;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	Window* CreateWindowsEngine = new Window();
	UIBuilder* ui = new UIBuilder();
	
	CreateWindowsEngine->StartWindow ("GameTubi Engine - 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1070, false);

	while (CreateWindowsEngine->running()) {
		CreateWindowsEngine->OnUpdate();
		Uint32 frameStart = SDL_GetTicks();

		CreateWindowsEngine->OnUpdate();
		ui->Update();

		Uint32 frameTime = SDL_GetTicks() - frameStart;

		int adjustedDelay = frameDelay - frameTime;
		if (adjustedDelay > 0) {
			SDL_Delay(adjustedDelay);
		}
	}

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
		ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->EndObject();
	}

	PlayMode::release					();
	ConsoleManager::release			();
	SaveData::release					();
	UIBuilder::release					();
	InputSystem::release				();
	ManagerScene::release			();
	CreateWindowsEngine->OnClean	();
	return 0;
}