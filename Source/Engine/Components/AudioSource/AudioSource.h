#pragma once
#include <iostream>
#include "irrKlang.h"
#include "../Gizmos/GizmoDrawer.h"

using namespace irrklang;
using namespace std;

class AudioSource
{
	ISoundEngine* engine = createIrrKlangDevice();
	GizmoDrawer* drawer = new GizmoDrawer();

public:
	void Start() {
		if (drawer != nullptr) {
			drawer->Start("AudioSource");
		}
	}

	void Play (string SoundName) {	
		if (!engine) {
			std::cout << "Failed to create mp3" << endl;
		}
		string path = "Assets/Audio/" + SoundName + ".mp3";
		engine->play2D(path.c_str(), false);
	}

	void Draw (float posX, float posY) {
		drawer->Draw (posX, posY);
	}
};