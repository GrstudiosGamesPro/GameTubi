#pragma once
#include "../SceneManager/ManagerScene.h"
#include "../AudioSource/AudioSource.h"
#include "../ParticlesSystem/ParticlesSystem.h"
#include <iostream>


using namespace std;

class ObjectManager
{
public:

	Object* FindObjectPerName (string Name) {

		for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
			if (ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetName() == Name) {
				return ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];
			}
		}

		return nullptr;
	}

	AudioSource* FindAudioSourcePerName (string SourceName) {
		for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Audio.size(); i++) {
			if (ManagerScene::GetInstance()->GetCurrentScene()->Audio[i]->Name == SourceName) {
				return ManagerScene::GetInstance()->GetCurrentScene()->Audio[i];
			}
		}
		return nullptr;
	}
};