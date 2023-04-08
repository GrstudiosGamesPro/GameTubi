#pragma once
#include "../Object.h"
#include "sol/sol.hpp"
#include "../Mathf/Vector2.h"
#include "../AudioSource/AudioSource.h"
#include "sol/sol.hpp"
#include "box2d.h"
#include "../InputSystem/InputSystem.h"
#include "../Console/ConsoleManager.h"
#include <iostream>
#include <any>
#include "../SceneManager/ManagerScene.h"

using namespace std;
using namespace sol;


class Lua
{
public:
	bool DenieCreateAudioSource;
	bool DenieCreateObjectManager;
	bool DenieCompileLua;
	bool LuaCompiled;
	sol::state lua;

	void CallLua(string ScriptToRun) {
		std::string codigo_lua = ScriptToRun;

		if (!DenieCompileLua) {
			if (!LuaCompiled) {
				lua.set_function("printF", [](sol::variadic_args args) {
					std::cout << "";
					for (auto arg : args) {
						std::cout << arg.as<float>() << " ";
					}
					std::cout << std::endl;
					});

				lua.set_function("printI", [](sol::variadic_args args) {
					std::cout << "";
					for (auto arg : args) {
						std::cout << arg.as<int>() << " ";
					}
					std::cout << std::endl;
					});

				lua.set_function("printSTR", [](sol::variadic_args args) {
					std::cout << "";
					for (auto arg : args) {
						std::cout << arg.as<string>() << " ";
					}
					std::cout << std::endl;
					});

				lua.set_function("printB", [](sol::variadic_args args) {
					std::cout << "";
					for (auto arg : args) {
						std::cout << arg.as<bool>() << " ";
					}
					std::cout << std::endl;
					});

				lua.set_function("print", [](sol::variadic_args args) {
					std::cout << "";
					for (auto arg : args) {
						ConsoleManager::GetInstance()->CreateLog(arg.as <string>());
					}
					std::cout << std::endl;
					});


				lua.new_usertype<Vector2>("Vector2", sol::constructors<Vector2(float, float)>(),
					"x", &Vector2::x,
					"y", &Vector2::y);


				lua["Vector2"]["new"] = [](float x, float y) {
					return Vector2(x, y);
				};

				lua["tostring"] = [](any obj) {
					if (obj.type() == typeid(string)) {
						return std::any_cast<string> (obj);
					}

					if (obj.type() == typeid(int)) {
						return to_string(std::any_cast<int> (obj));
					}

					if (obj.type() == typeid(float)) {
						return to_string(std::any_cast<float> (obj));
					}
				};


				lua.new_usertype<Object>("Object",
					"SetPosition", &Object::SetPosition,
					"GetPosition", &Object::GetPosition,
					"Name", &Object::name,
					"GetName", &Object::GetName,
					"GetScale", &Object::GetScale,
					"SetNewScale", &Object::SetNewScale,
					"SetActive", &Object::SetActive,
					"SetScaleBox", &Object::SetScaleBox,
					"GetScaleBox", &Object::GetScaleBox,
					"SetDensityBox", &Object::SetDensityBox,
					"GetDensityBox", &Object::GetDensityBox,
					"SetFrictionBox", &Object::SetFrictionBox,
					"GetFrictionBox", &Object::GetFrictionBox,
					"AddForce", &Object::AddForce,
					"UseGravity", &Object::useGravity
				);

				if (ManagerScene::GetInstance() != nullptr) {
					lua.new_usertype<ManagerScene>("Scene");
					lua["Scene"]["LoadScene"] = [](string SceneName) { ManagerScene::GetInstance()->GetCurrentScene()->LoadScene(SceneName);  };
				}

				lua["Object"]["new"] = []() { return ManagerScene::GetInstance()->GetCurrentScene()->SetupNewObject(); };
				//lua["Object"]["FindObjectPerName"] = [](string FindName) {return OBJManager->FindObjectPerName(FindName); };

				if (!DenieCreateAudioSource) {
					sol::usertype<AudioSource> miTipo = lua.new_usertype<AudioSource>("AudioSource",
						"Play", &AudioSource::Play,
						"Stop", &AudioSource::Stop,
						"SetVolumen", &AudioSource::SetVolume,
						"GetVolumen", &AudioSource::GetVolumen,
						"PlayPerName", &AudioSource::PlayClipPerName,
						"SetPosition", &AudioSource::SetPosition
					);

					//lua["AudioSource"]["FindSourcePerName"] = [](string FindName) {return OBJManager->FindAudioSourcePerName(FindName); };
					//lua["AudioSource"] = source;
				}



				if (InputSystem::GetInstance() != nullptr) {
					lua.set_function("OnKeyDown", [](const string& keyName) -> bool {
						return InputSystem::GetInstance()->OnKeyDown(keyName);
						});
				}
			}

			int result = luaL_loadstring(lua.lua_state(), codigo_lua.c_str());
			if (result == LUA_OK) {
				if (!LuaCompiled) {
					//Object* OBJ = GetObject();
					//lua["self"] = sol::make_object(lua.lua_state(), OBJ);
				}

				result = lua_pcall(lua.lua_state(), 0, LUA_MULTRET, 0);
				if (result != LUA_OK) {
					std::string error = lua_tostring(lua.lua_state(), -1);
					ConsoleManager::GetInstance()->CreateLog(error);
					std::cout << "Error: " << error << std::endl;
					lua_pop(lua.lua_state(), 1);
					LuaCompiled = false;
				}
				else {
					LuaCompiled = true;
					std::cout << "Lua compiled " << endl;
				}
			}
			else {
				std::string error = lua_tostring(lua.lua_state(), -1);
				std::cout << "Error: " << error << std::endl;
				ConsoleManager::GetInstance()->CreateLog(error);
				lua_pop(lua.lua_state(), 1);

				ConsoleManager::GetInstance()->CreateLog("Lua error on compiled ");
			}
		}
	}
};