#pragma once
#include <iostream>
#include "irrKlang.h"
#include "../Gizmos/GizmoDrawer.h"
#include "../Mathf/Vector2.h"

using namespace irrklang;
using namespace std;

class AudioSource
{
	ISoundEngine* engine = createIrrKlangDevice();

public:
	Vector2 Position;
	string Name = "Audio Source";
	string AudioPath;
	GizmoDrawer* drawer = new GizmoDrawer();

	void Start() {
		if (drawer != nullptr) {
			drawer->Start("AudioSource");
		}
	}

	void Play () {
		if (!engine) {
			std::cout << "Failed to create mp3" << endl;
		}
		string path = "Assets/Audio/" + AudioPath + ".mp3";
		engine->play2D(path.c_str(), false);
	}

	void PlayClipPerName (string SoundName) {
		if (!engine) {
			std::cout << "Failed to create mp3" << endl;
		}
		string path = "Assets/Audio/" + SoundName + ".mp3";
		engine->play2D(path.c_str(), false);
	}

	void SetPosition (float PosX, float PosY) {
		Position = Vector2 (PosX, PosY);
	}

	void Draw() {
		drawer->Draw(Position.x, Position.y);
	}

	void Stop() {
		engine->stopAllSounds();
	}

	void SetVolume(float Volumen) {
		engine->setSoundVolume(Volumen);
	}

	float GetVolumen() {
		return engine->getSoundVolume();
	}


	~AudioSource() {
		delete (drawer);
		delete (engine);
	}
};