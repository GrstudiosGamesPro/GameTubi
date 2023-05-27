#pragma once
#include "../Window.h"
#include "SDL.h"
#include "Mathf/Vector2.h"
#include "AudioSource/AudioSource.h"
#include "sol/sol.hpp"
#include "box2d.h"
#include "InputSystem/InputSystem.h"

using namespace std;
using namespace sol;

struct b2Body;
struct b2PolygonShape;
struct b2BodyDef;
struct b2FixtureDef;

class Object
{
private:
	static SDL_Event events;
	bool m_luaOK = false;
	float NewPosX, NewPosY;

public:
	SDL_Rect srcRect, destRect;
	SDL_Texture* text;
	b2Body* body;
	b2PolygonShape* dynamicBox;
	b2FixtureDef* fixtureDef;
	AudioSource* source;
	sol::state lua;

	bool LuaCompiled;
	Vector2 pos;
	string name = "Game Object";
	string Tag = "Default";

	std::string TexturePath;
	std::string Script = "function Start ()\n\n--EXECUTE ON START THE GAME\n\n\nend\n\n\n\n\n\n\n\nfunction Tick ()\n\n--THIS EXECUTE IN LOOP \n\n\nend";
	int height = 32;
	int width = 32;
	float ScaleX = 1;
	float ScaleY = 1;
	float ScaleBoxX = 1;
	float ScaleBoxY = 1;
	int scale = 1;
	float Angle = 0;
	bool isActive = true;
	bool IsTrigger = false;
	bool UsePhysics;
	bool useGravity = false;
	float density = 1.0f;
	float friction = 0.3f;
	float RectTextureX, RectTextureY;
	bool IsStatic;
	bool DenieCreateAudioSource;
	bool DenieCreateObjectManager;
	bool DenieCompileLua;
	bool ControlAngleBody = false;
	bool ChangingChild = false;
	bool Triggered;
	bool RunLua;

	//Animation options
	bool IsAnimation = false;
	int frames = 0;
	int speed = 100;
	bool UseMusic;


	Object* Parent;
	vector<Object*> Childrens = vector<Object*>();

	string ObjectID = "____________________________";

	Object();

	void Start();
	void Update();

	void Draw();
	void SetPosition (float x, float y);
	void SetName   (string name);
	void SetNewTexture();
	void CompileLua();
	void UpdateCollisions();
	void CreateBody();
	void Cleanup();
	void EndObject();
	void UpdateBody();
	void SetNewScale (float x, float y);
	void SetActive (bool t);
	void SetScaleBox(float x, float y);
	void SetDensityBox  (float x);
	void SetFrictionBox(float x);
	void AddForce(float x, float y);
	void CallLua(string scr);
	void RemoveFromParent();
	void OnTriggerStart (Object* Other);
	void OnTriggerEnd (Object* Other);

	bool AlreadyFunctionsRegister;
	bool m_IsStatic();


	float GetDensityBox();
	float GetFrictionBox();

	Vector2 GetPosition();
	SDL_Texture* GetTexture();
	string GetName();
	SDL_Rect* GetRectSRC();
	SDL_Rect* GetRectDEST();
	Vector2 GetScale ();
	Vector2 GetScaleBox ();
	Object* GetObject();

	void Clear();


	~Object();
};