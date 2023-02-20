#pragma once
#include "../Object.h"
#include <iostream>
#include <vector>
#include "box2d.h"
#include "b2_draw.h"
#include "../SaveSystem/SaveData.h"

using namespace std;

struct b2Draw;

class Scene
{
public:
	SDL_Surface* background;
	SDL_Texture* backgroundTexture;
	std::string TexturePath = "Assets/Sprites/Background.png";

	std::vector<Object*> ObjectsInScene;
	b2World* GravityWorld;
	string SceneName = "SampleScene";

	void OnStartScene() {
		background = IMG_Load(TexturePath.c_str());
		backgroundTexture = SDL_CreateTextureFromSurface(Window::renderer, background);
		SDL_FreeSurface(background);


		b2Vec2 gravity(0, 20);
		GravityWorld = new b2World(gravity);

		if (GravityWorld != nullptr) {
			std::cout << "Gravity created " << endl;
		}

		SaveData::GetInstance()->Load();
	}

	void AddNewBodyWorld() {

	}

	void UpdateScene() {
		if (GravityWorld != nullptr) {
			if (GravityWorld->GetBodyCount() > 0) {
				float timeStep = 1.0f / 60.0f;
				GravityWorld->Step(timeStep, 6, 2);

				b2Body* body = GravityWorld->GetBodyList();
				body->GetNext();
			}
		}
	}
	void SetupNewObject () {
		Object* newOBJ = new Object();
		newOBJ->TexturePath = "Assets/Sprites/idle.gif";
		newOBJ->Start();
		newOBJ->SetNewTexture();
		ObjectsInScene.push_back (newOBJ);
	}



	void SaveScene () {
		SaveData::GetInstance()->Save();
	}

	void SetNewBackground () {
		background = IMG_Load(TexturePath.c_str());
		backgroundTexture = SDL_CreateTextureFromSurface(Window::renderer, background);
		SDL_FreeSurface(background);
		std::cout << "Background Changed " << endl;
	}

	void LoadScene() {

	}

};