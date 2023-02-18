#include "InputSystem.h"

InputSystem* InputSystem::m_engine = nullptr;

InputSystem::InputSystem() {

}

void InputSystem::create()
{
	if (InputSystem::m_engine) throw std::exception("InputSystem already created.");
	InputSystem::m_engine = new InputSystem();
	std::cout << "InputSystem Created" << endl;
}


InputSystem* InputSystem::GetInstance()
{
	return m_engine;
}

void InputSystem::release()
{
	if (!InputSystem::m_engine) return;
	delete InputSystem::m_engine;
}

void InputSystem::SetNewInput (string Name, string Key, string Axis, string NegateAxis) {
	Inputs gms = Inputs();
	gms.Name = Name;
	gms.Key = Key;
	gms.Axis = Axis;
	gms.NegateAxis = NegateAxis;

	inputs.push_back (gms);
}

InputSystem::~InputSystem()
{
	InputSystem::m_engine = nullptr;
}