#pragma once
#include "../Object.h"
#include "../Object/ObjectManager.h"
#include "../Mathf/Vector2.h";
#include <iostream>
using namespace std;

class LuaRunner
{
public:
	LuaRunner() {

	}
	~LuaRunner() {

	}

	Object* owner;
	ObjectManager* objs = new ObjectManager();

	
};