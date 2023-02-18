#pragma once

#include <iostream>
#include <vector>

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

public:
	struct Inputs {
		string Name;
		string Key;
		string Axis;
		string NegateAxis;
	};


	vector<Inputs> inputs;

public: 
	void SetNewInput (string Name, string Key, string Axis, string NegateAxis);
};