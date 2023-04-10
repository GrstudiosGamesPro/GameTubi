#pragma once
#include "box2d.h"
#include <iostream>

using namespace std;

class TriggerData : public b2ContactListener
{
public:
	void BeginContact	(b2Contact* contact);
	void EndContact		(b2Contact* contact);
};