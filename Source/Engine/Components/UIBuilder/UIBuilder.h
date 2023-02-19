#pragma once
#include "SDL.h"

#include <iostream>

using namespace std;

class UIBuilder
{
private:
	static UIBuilder* m_engine;


public:
	UIBuilder();
	~UIBuilder();

	static void create();
	static void release();
	static void Update();
	static UIBuilder* GetInstance();
};