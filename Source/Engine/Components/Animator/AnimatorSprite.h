#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

class AnimatorSprite
{
public:
	vector<string*> Sprites = vector<string*>();


	void PlayingAnimation () {

	}

	void SetupNewTexture (string TextureName) {
		string* newPath = new string;
		Sprites.push_back(newPath);
	}
};