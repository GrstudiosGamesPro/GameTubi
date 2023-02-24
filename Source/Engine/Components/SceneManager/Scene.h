#pragma once
#include "../Object.h"
#include <iostream>
#include <vector>
#include "box2d.h"
#include "b2_draw.h"
#include "../SaveSystem/SaveData.h"
#include "../AudioSource/AudioSource.h"

using namespace std;

struct b2Draw;

class Scene
{
public:
	SDL_Surface* background;
	SDL_Texture* backgroundTexture;

	std::string TexturePath = "Assets/Sprites/Background.png";

	std::vector<Object*> ObjectsInScene;
	std::vector<AudioSource*> Audio;
	b2World* GravityWorld;
	string SceneName = "SampleScene";
	bool UseFullScreen = false;

	void OnStartScene() {
		background = IMG_Load(TexturePath.c_str());
		backgroundTexture = SDL_CreateTextureFromSurface(Window::renderer, background);
		SDL_FreeSurface(background);

		CreateGravity();


		Scene::LoadScene("SampleScene");
	}

	void CreateGravity() {
		b2Vec2 gravity(0, 20);
		GravityWorld = new b2World(gravity);

		if (GravityWorld != nullptr) {
			std::cout << "Gravity created " << endl;
		}
	}

	void UpdateScene() {
		if (GravityWorld != nullptr && GravityWorld->GetBodyCount() > 0) {
			b2Body* body = GravityWorld->GetBodyList();
			float timeStep = 1.0f / 60.0f;
			GravityWorld->Step(timeStep, 6, 2);
			body->GetNext();
		}
	}

	Object* SetupNewObject () {
		Object* newOBJ = new Object();
		newOBJ->TexturePath = "Assets/Sprites/idle.gif";
		newOBJ->Start();
	    newOBJ->SetNewTexture();
		ObjectsInScene.push_back (newOBJ);

		return newOBJ;
	}

	AudioSource* SetupNewAudio() {
		AudioSource* NewAudio = new AudioSource();
		NewAudio->Name = "Audio Source";
		NewAudio->Start();
		Audio.push_back (NewAudio);
		return NewAudio;
	}

	void UnLoadAllBodys() {
		b2Body* nextBody = GravityWorld->GetBodyList();
		GravityWorld->DestroyBody(nextBody);
	}

	void SaveScene () {
		SaveData::GetInstance()->Save();
	}


	void LoadScene (string SceneName) {
		string Path = "Assets/SaveData/" + SceneName + ".Scene";
		SaveData::GetInstance()->Load (Path);
	}

	void SetNewBackground () {
		background = IMG_Load(TexturePath.c_str());
		backgroundTexture = SDL_CreateTextureFromSurface(Window::renderer, background);
		SDL_FreeSurface(background);
		std::cout << "Background Changed " << endl;
	}
};