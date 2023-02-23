#pragma once
#include "../SceneManager/ManagerScene.h"
#include <iostream>

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
};