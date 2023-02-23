#pragma once
#include <iostream>;
#include "Scene.h";

using namespace std;

class ManagerScene
{
private:
	static ManagerScene* m_engine;
	Scene* CurrentScene = new Scene();


public:
	ManagerScene();
	~ManagerScene();

	static void create();
	static void release();
	static ManagerScene* GetInstance();

	Scene* GetCurrentScene();

	Scene* LoadScene (string SceneName);
};