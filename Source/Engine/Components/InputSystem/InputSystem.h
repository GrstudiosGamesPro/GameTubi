#pragma once

#include <iostream>
#include <vector>
#include "SDL.h"

using namespace std;

class InputSystem
{
public:
	InputSystem();
	~InputSystem();
	static InputSystem* m_engine;


	static void create();
	static void release();
	static InputSystem* GetInstance();
	SDL_Event* KeyEvent;

public:
	struct Inputs {
		string Name;
		string Key;
		string NegateKey;
		string Axis;
		string NegateAxis;
	};


	vector<Inputs> inputs;

public: 
	void SetNewInput (string Name, string Key, string NegateKey, string Axis, string NegateAxis);
	bool OnKeyDown   (string keyCode);
	bool OnKeyUp     (string keyCode);
};