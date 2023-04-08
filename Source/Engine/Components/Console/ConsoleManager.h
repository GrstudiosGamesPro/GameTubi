#pragma once
#include <iostream>
#include <vector>

using namespace std;

class ConsoleManager
{


public:
	static ConsoleManager* m_engine;
	vector<string>* Logs = new vector<string>();
	ConsoleManager();
	~ConsoleManager();

	static void create();
	static void release();
	static ConsoleManager* GetInstance();
	void CreateLog (string logMessage);
	string GetLog (int index);
};