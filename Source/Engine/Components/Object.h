#pragma once
#include "../Window.h"
#include "SDL.h"
#include "Mathf/Vector2.h"
#include "lua.hpp"

using namespace std;

struct b2Body;
struct b2PolygonShape;
struct b2BodyDef;
struct b2FixtureDef;


class Object
{
private:
	static SDL_Event events;
	SDL_Texture* text;
	SDL_Rect srcRect, destRect;
	lua_State* m_PTRLuaState = nullptr;
	bool m_luaOK = false;
public:
	b2Body* body;
	b2PolygonShape* dynamicBox;
	b2FixtureDef* fixtureDef;

	Vector2 pos;
	string name = "Game Object";
	std::string TexturePath;
	std::string Script = "io.write ('NEW LUA SCRIPT')\n\n\n\n\n function OnStart ()\n\n--EXECUTE ON START THE GAME\n\n\nend\n\n\n\n\n\n\n\n function OnUpdate ()\n\n--THIS EXECUTE IN LOOP \n\n\nend";
	string tag;
	int height = 32;
	int width = 32;
	float ScaleX = 1;
	float ScaleY = 1;
	float ScaleBoxX = 1;
	float ScaleBoxY = 1;
	int scale = 1;
	float Angle = 0;
	bool isActive = true;
	bool useGravity = false;
	float density = 1.0f;
	float friction = 0.3f;
	

	Object();

	void Start();
	void Update();

	void Draw();
	Vector2 GetPosition();
	void SetPosition (float x, float y);
	void SetName   (string name);
	void SetNewTexture();
	void InputSystem();
	void CompileLua();
	void UpdateCollisions();
	void CreateBody();
	void UpdateCollisionState ();

	SDL_Texture* GetTexture();
	string GetName();

	SDL_Rect* GetRectSRC();
	SDL_Rect* GetRectDEST();

	~Object();

	//REGISTRAR FUNCIONES DE LUA
	int GetObjectName(lua_State* L) {
		char nameGet[512];

		strcpy_s(nameGet, name.c_str());

		std::string myString(nameGet);
		lua_pushstring(L, myString.c_str());
		return 1;
	}
};