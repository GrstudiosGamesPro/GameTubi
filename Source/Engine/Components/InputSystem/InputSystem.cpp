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

void InputSystem::SetNewInput (string Name, string Key, string NegateKey, string Axis, string NegateAxis) {
	Inputs gms = Inputs();
	gms.Name = Name;
	gms.Key = Key;
	gms.Axis = Axis;
	gms.NegateKey = NegateKey;
	gms.NegateAxis = NegateAxis;

	inputs.push_back (gms);
}


bool InputSystem::OnKeyUp(string KeyName) {
	if (KeyEvent != nullptr && KeyEvent->type == SDL_KEYUP) {
		for (Inputs g : inputs) {
			if (g.Name == KeyName) {
				if (SDL_GetKeyName(KeyEvent->key.keysym.sym) == g.Key) {
					return true;
				}
				else {
					return false;
				}
			}

		}
	}
	return false;
}

bool InputSystem::OnKeyDown(string KeyName) {
	if (KeyEvent != nullptr && KeyEvent->type == SDL_KEYDOWN) {
		for (Inputs g : inputs) {
			if (g.Name == KeyName) {
				if (SDL_GetKeyName(KeyEvent->key.keysym.sym) == g.Key) {
					return true;
				}
				else {
					return false;
				}
			}

		}
	}
	return false;
}


InputSystem::~InputSystem()
{
	InputSystem::m_engine = nullptr;
}