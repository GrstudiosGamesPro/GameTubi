#pragma once
#include "../Object.h"
#include <iostream>
#include <vector>
#include "box2d.h"
#include "b2_draw.h"

using namespace std;

struct b2Draw;

class Scene
{
public:
	std::vector<Object*> ObjectsInScene;
	b2World* GravityWorld;


	void OnStartScene() {

		b2Vec2 gravity(0, 20);
		GravityWorld = new b2World(gravity);

		if (GravityWorld != nullptr) {
			std::cout << "Gravity created " << endl;
		}

		Object* newOBJ = new Object();
		newOBJ->TexturePath = "Assets/Sprites/idle.gif";
		newOBJ->Start();
		newOBJ->SetName("Psass");

		newOBJ->SetNewTexture();
		ObjectsInScene.push_back(newOBJ);

		Object* newOBJ2 = new Object();
		newOBJ2->TexturePath = "Assets/Sprites/idle.gif";
		newOBJ2->Start();
		newOBJ2->SetName ("Ps2");
		newOBJ2->SetNewTexture();
		ObjectsInScene.push_back(newOBJ2);

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
};