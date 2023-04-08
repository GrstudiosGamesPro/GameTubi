#include "ConsoleManager.h"
using namespace std;

ConsoleManager* ConsoleManager::m_engine = nullptr;

ConsoleManager::ConsoleManager() {

}

void ConsoleManager::create()
{
	if (ConsoleManager::m_engine) throw std::exception("ConsoleManager is already created.");
	ConsoleManager::m_engine = new ConsoleManager();
	std::cout << "ConsoleManager Created" << endl;
}


ConsoleManager* ConsoleManager::GetInstance()
{
	return m_engine;
}

void ConsoleManager::release()
{
	if (!ConsoleManager::m_engine) return;
	delete ConsoleManager::m_engine;
}

ConsoleManager::~ConsoleManager()
{
	ConsoleManager::m_engine = nullptr;
}

void ConsoleManager::CreateLog(string logMessage) {
	Logs->push_back (logMessage);
	string nm = logMessage;
	std::cout << "Log added. (" << nm << ") " << endl;
}

string ConsoleManager::GetLog(int index) {
	return (*Logs)[index];
}