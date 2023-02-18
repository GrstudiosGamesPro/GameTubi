#include "ManagerScene.h"
#include <iostream>

using namespace std;

ManagerScene* ManagerScene::m_engine = nullptr;

ManagerScene::ManagerScene() {

}

void ManagerScene::create()
{
	if (ManagerScene::m_engine) throw std::exception ("SceneManager already created.");
	ManagerScene::m_engine = new ManagerScene();
	std::cout << "SceneManager Created" << endl;
}

Scene* ManagerScene::GetCurrentScene() {
	return CurrentScene;
}

ManagerScene* ManagerScene::GetInstance()
{
	return m_engine;
}

void ManagerScene::release()
{
	if (!ManagerScene::m_engine) return;
	delete ManagerScene::m_engine;
}

ManagerScene::~ManagerScene()
{
	ManagerScene::m_engine = nullptr;
}