#pragma once
#include "Object.h"
#include "box2d.h"
#include "SceneManager/ManagerScene.h"
#include "SaveSystem/SaveData.h"
#include "Object/ObjectManager.h"
#include "Console/ConsoleManager.h"
#include "Mathf/GMathf.h"

#include <any>

static ObjectManager* OBJManager;

Object::Object () {

}

 
void Object::Start() {
	//text = TextureManager::LoadTexture("Assets/Sprites/idle.gif");
	//TexturePath = "Assets/Sprites/idle.gif";

	/*
	srcRect.x = srcRect.y = 0;
	srcRect.w = width;
	srcRect.h = height;
	destRect.w = width * ScaleX;
	destRect.h = height * ScaleY;
	*/

	Object::CreateBody();
	body->SetTransform(b2Vec2(pos.x, pos.y), 0);

	if (!DenieCreateObjectManager) {
		OBJManager = new ObjectManager();
	}

	if (!DenieCreateAudioSource) {
		//source = new AudioSource();
		//source->Start();
	}

	body->SetType(b2_staticBody);

	int textureWidth, textureHeight;
	SDL_QueryTexture(text, nullptr, nullptr, &textureWidth, &textureHeight);
	TexturePath = TexturePath.c_str();
	srcRect.w = textureWidth;
	srcRect.h = textureHeight;
	destRect.w = textureWidth * ScaleX;
	destRect.h = textureHeight * ScaleY;
	width = textureHeight;
	height = textureHeight;
}

void Object::Update() {
	if (isActive) {
		b2Vec2 center = body->GetWorldCenter();

		int textureWidth = width;
		int textureHeight = height;

		float HalfTextureWidth = textureWidth / 2.0f;
		float HalfTextureHeigh = textureHeight / 2.0f;

		float textureX = center.x * width - HalfTextureWidth;
		float textureY = center.y * height - HalfTextureHeigh;
		

		float centerX = body->GetPosition().x * 32;
		float centerY = body->GetPosition().y * 32;

		textureWidth = width * ScaleX;
		textureHeight = height * ScaleY;
		textureX = centerX - textureWidth / 2;
		textureY = centerY - textureHeight / 2;

		RectTextureX = textureX;
		RectTextureY = textureY;

		if (!IsStatic) {
			if (useGravity && body != nullptr)
			{
				body->SetAwake(true);
				body->SetType(b2_dynamicBody);
			}
			else {
				if (body != nullptr) {
					body->SetType(b2_staticBody);
				}
			}

			destRect.x = textureX - Window::camera.x;
			destRect.y = textureY - Window::camera.y;

			destRect.w = width * ScaleX;
			destRect.h = height * ScaleY;

			pos.x = body->GetPosition().x;
			pos.y = body->GetPosition().y;
		}
		else {
			if (body != nullptr) {
				body->SetType(b2_staticBody);
			}
		}

		if (IsTrigger) {

		}

		if (LuaCompiled) {
			lua["Tick"]();
		}

		if (IsAnimation) {
			//srcRect.x = srcRect.w * static_cast<int> ((SDL_GetTicks() / speed) % frames);
		}
	}
}

void Object::Cleanup() {
	ManagerScene::GetInstance()->GetCurrentScene()->DeleteBody(this->body);
	SDL_DestroyTexture (text);
}


void Object::CreateBody() {

	if (ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld != nullptr) {
		if (dynamicBox == nullptr) {
			dynamicBox = new b2PolygonShape();
		}
		if (fixtureDef == nullptr) {
			fixtureDef = new b2FixtureDef();
		}

		b2BodyDef bodyDef;

		bodyDef.type = b2_dynamicBody;
		fixtureDef->shape = dynamicBox;
		fixtureDef->density = density;
		fixtureDef->friction = friction;
		body = ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->CreateBody(&bodyDef);

		b2Vec2 localCenter(0.0f, 0.0f);
		localCenter.Set(0, 0);
		Object::UpdateCollisions();
		fixtureDef->shape = dynamicBox;
		body->CreateFixture(fixtureDef);
	}
}

void Object::UpdateBody() {
	b2PolygonShape* dynamicBox = new b2PolygonShape;
	int texWidth = width;
	int texHeight = height;
	b2Vec2 localCenter(0.0f, 0.0f);
	localCenter.Set(0, 0);
	float scaleX = (float)ScaleBoxX;
	float scaleY = (float)ScaleBoxY;
	float boxWidth = (float)texWidth * scaleX / (float)width / 2.0f;
	float boxHeight = (float)texHeight * scaleY / (float)height / 2.0f;
	dynamicBox->SetAsBox(float(width) * boxWidth / width, float(height) * boxHeight / height, localCenter, 0);

	b2FixtureDef fxd;
	fxd.shape = dynamicBox;
	fxd.density = 1.0f;
	fxd.friction = 0.3f;
	fxd.isSensor = true;

	body->DestroyFixture (body->GetFixtureList());
	body->GetFixtureList()->SetSensor (IsTrigger);
	body->CreateFixture (&fxd);
}


void Object::UpdateCollisions() {
	float textureCenterX = width / 2.0f * ScaleBoxX;
	float textureCenterY = height / 2.0f * ScaleBoxY;
	b2Vec2 localCenter (0.0f, 0.0f);
	localCenter.Set (0, 0);

	int texWidth = width;
	int texHeight = height;
	body->DestroyFixture(body->GetFixtureList());

	b2FixtureDef* fx = new b2FixtureDef();
	float scaleX = (float)ScaleBoxX;
	float scaleY = (float)ScaleBoxY;
	fx->density = density;
	fx->friction = friction;

	float boxWidth = (float)texWidth * scaleX / (float)width / 2.0f;
	float boxHeight = (float)texHeight * scaleY / (float)height / 2.0f;
	dynamicBox->SetAsBox (float(width) * boxWidth / width, float(height) * boxHeight / height, localCenter, 0);
	fx->shape = dynamicBox;
	fixtureDef = fx;
	body->CreateFixture(fx);
}


string Object::GetName() {
	return name;
}

void Object::SetName (string _name) {
	name = _name;
}


void Object::SetPosition(float x, float y) {
	body->SetTransform(b2Vec2((float)x, (float)y), 0);
}

Vector2 Object::GetPosition() {
	
	if (body != nullptr) {
		return Vector2(pos.x, pos.y);
	}

	return Vector2(body->GetPosition().x, body->GetPosition().y);
}

void Object::Draw() {
	if (isActive) {

		if (ControlAngleBody) {
			Angle = body->GetAngle();
		}

		b2PolygonShape* shape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
		b2Vec2 vertices[b2_maxPolygonVertices];
		TextureManager::Draw(text, srcRect, Angle, destRect);

		for (int i = 0; i < shape->m_count; i++) {
			vertices[i] = body->GetWorldPoint(shape->m_vertices[i]);
		}

		// Convertir los vértices de Box2D a píxeles
		int pixelVertices[b2_maxPolygonVertices * 2];
		for (int i = 0; i < shape->m_count; i++) {
			pixelVertices[i * 2] = (int)(vertices[i].x * 32 - Window::camera.x);
			pixelVertices[i * 2 + 1] = (int)(vertices[i].y * 32 - Window::camera.y);
		}

		// Dibujar el cuadrado en SDL
		SDL_SetRenderDrawColor(Window::renderer, 255, 25, 0, 255);
		for (int i = 0; i < shape->m_count; i++) {
			int j = (i + 1) % shape->m_count;
			SDL_RenderDrawLine(Window::renderer, pixelVertices[i * 2], pixelVertices[i * 2 + 1], pixelVertices[j * 2], pixelVertices[j * 2 + 1]);
		}



		/*
		float textureCenterX = width / 2.0f * ScaleX;
		float textureCenterY = height / 2.0f * ScaleY;
		b2Vec2 localCenter(0.0f, 0.0f);
		localCenter.Set(textureCenterX, textureCenterY);

		int x = body->GetPosition().x;
		int y = body->GetPosition().y;

		int thickness = 5;
		float angle = Angle; // Ángulo de rotación en grados

		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(Window::renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(Window::renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

		// Definir puntos del rectángulo
		SDL_Point points[5] = {
			{ x, y },
			{ x + ScaleX * width, y },
			{ x + ScaleX * width, y + ScaleY * height },
			{ x, y + ScaleY * height },
			{ x, y }
		};


		// Rotar los puntos del rectángulo
		SDL_Point center = { body->GetPosition().x, body->GetPosition().y}; // Punto central del rectángulo
		for (int i = 0; i < 5; i++) {
			float s = sin(angle * M_PI / 180);
			float c = cos(angle * M_PI / 180);
			int xnew = (int)(center.x + (points[i].x - center.x) * c - (points[i].y - center.y) * s);
			int ynew = (int)(center.y + (points[i].x - center.x) * s + (points[i].y - center.y) * c);
			points[i] = { xnew, ynew };
		}

		// Dibujar el rectángulo rotado
		SDL_RenderDrawLines(Window::renderer, points, 5);

		SDL_SetRenderDrawColor(Window::renderer, r, g, b, a);*/

	}
}

void Object::SetNewTexture() {
	SDL_DestroyTexture (text);
	text = TextureManager::LoadTexture(TexturePath.c_str());
	int textureWidth, textureHeight;
	SDL_QueryTexture(TextureManager::LoadTexture(TexturePath.c_str()), nullptr, nullptr, &textureWidth, &textureHeight);
	TexturePath = TexturePath.c_str();
	srcRect.w = textureWidth;
	srcRect.h = textureHeight;
	destRect.w = textureWidth * ScaleX;
	destRect.h = textureHeight * ScaleY;
	width = textureHeight;
	height = textureHeight;
}


Object::~Object() {
	delete (&body);
	delete (lua);
	delete (this);

	ManagerScene::GetInstance()->GetCurrentScene()->DeleteBody(this->body);

	SDL_DestroyTexture(text);
}

SDL_Texture* Object::GetTexture() {
	return text;
}

SDL_Rect* Object::GetRectDEST() {
	return &destRect;
}

SDL_Rect* Object::GetRectSRC() {
	return &srcRect;
}


void Object::SetNewScale (float x, float y) {
	ScaleX = x;
	ScaleY = y;
}

Vector2 Object::GetScale() {
	return Vector2 (ScaleX, ScaleY);
}


void Object::EndObject() {

}

bool Object::m_IsStatic (){
	return IsStatic;
}


void Object::CompileLua() {
	Object::CallLua (Script);
}

void Object::SetActive (bool t) {
	isActive = t;
}

void Object::SetScaleBox (float x, float y) {
	ScaleBoxX = x;
	ScaleBoxY = x;
	UpdateCollisions();
}

Vector2 Object::GetScaleBox () {
	return Vector2 (ScaleBoxX, ScaleBoxY);
}

void Object::SetDensityBox (float x) {
	density = x;
	UpdateCollisions();
}

float Object::GetDensityBox() {
	return density;
}

void Object::SetFrictionBox(float x) {
	friction = x;
	UpdateCollisions();
}

float Object::GetFrictionBox() {
	return friction;
}

void Object::AddForce (float x, float y) {
	body->ApplyForce (b2Vec2 (x, y), body->GetPosition(), true);
}

Object* Object::GetObject() {
	return this;
}


void Object::OnTriggerStart (Object* other) {
	std::cout << "Entrando en la colision " << other->name << endl; 
}

void Object::OnTriggerEnd (Object* other) {
	std::cout << "Saliendo de la colision" << endl;
}

void Object::RemoveFromParent() {
	for (int e = 0; e < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); e++) {

		Object* CurrentOBJ = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[e];

		for (int i = 0; i < CurrentOBJ->Childrens.size(); i++) {
			if (this == CurrentOBJ->Childrens[i]) {
				std::vector<Object*>::iterator it = CurrentOBJ->Childrens.begin() + i;
				CurrentOBJ->Childrens.erase(it);
				std::cout << "Se ah removido del padre" << endl;
			}
		}
	}
}


void Object::CallLua(string ScriptToRun) {
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
			lua["Object"]["FindObjectPerName"] = [](string FindName) {return OBJManager->FindObjectPerName(FindName); };

				sol::usertype<AudioSource> miTipo = lua.new_usertype<AudioSource>("AudioSource",
					"Play", &AudioSource::Play,
					"Stop", &AudioSource::Stop,
					"SetVolumen", &AudioSource::SetVolume,
					"GetVolumen", &AudioSource::GetVolumen,
					"PlayPerName", &AudioSource::PlayClipPerName,
					"SetPosition", &AudioSource::SetPosition,
					"AudioPath", &AudioSource::AudioPath,
					"Name", &AudioSource::Name,
					"Position", &AudioSource::SetPosition
				);

				lua["AudioSource"]["FindSourcePerName"] = [](string FindName) {return OBJManager->FindAudioSourcePerName(FindName); };
				//lua["AudioSource"] = source;

			lua.new_usertype<GMathf>("GMathf");
			lua["GMathf"]["ClampFloat"] = [](float value, float min, float max) { return  GMathf::ClampFloat(value, min, max);  };
			lua["GMathf"]["ClampInt"] = [](int value, int min, int max) { return  GMathf::ClampFloat(value, min, max); };
			lua["GMathf"]["Clamp01"] = [](float value) { return  GMathf::Clamp01(value); };
			lua["GMathf"]["Random"] = [](float min, float max) { return  GMathf::RandomFloat (min, max); };
			lua["GMathf"]["Abs"] = [](float Value) { return  GMathf::ABS(Value); };
			lua["GMathf"]["Aproximity"] = [](float a, float b, float tolerance) { return  GMathf::Aproximity(a, b, tolerance); };
			lua["GMathf"]["Asin"] = [](float Value) { return  GMathf::ASIN(Value); };
			lua["GMathf"]["Atan"] = [](float Value) { return  GMathf::Atan(Value); };
			lua["GMathf"]["Atan2"] = [](float x, float y) { return  GMathf::Atan2(x, y); };
			lua["GMathf"]["Ceil"] = [](float Value) { return  GMathf::Ceil(Value); };
			lua["GMathf"]["ClosestPowerOfTwo"] = [](float Value) { return  GMathf::ClosestPowerOfTwo(Value); };
			lua["GMathf"]["Sqrt"] = [](float Value) { return  GMathf::SQRT(Value); };
			lua["GMathf"]["SmoothDamp"] = [](float current, float target, float currentVelocity, float smoothTime, float maxSpeed, float deltaTime) { return  GMathf::SmoothDamp (current, target, currentVelocity, smoothTime, maxSpeed, deltaTime); };
			lua["GMathf"]["SmoothDampAngle"] = [](float current, float target, float currentVelocity, float smoothTime, float maxSpeed, float deltaTime) { return  GMathf::SmoothDampAngle(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime); };			lua["GMathf"]["SmoothDampAngle"] = [](float current, float target, float currentVelocity, float smoothTime, float maxSpeed, float deltaTime) { return  GMathf::SmoothDampAngle(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime); };
			lua["GMathf"]["PI"] = []() { return  GMathf::PI(); };
			lua["GMathf"]["SmoothStep"] = [](double start, double end, double value) { return  GMathf::SmoothStep (start, end, value); };
			lua["GMathf"]["Tan"] = [](double degrees) { return  GMathf::Tan(degrees); };



			if (InputSystem::GetInstance() != nullptr) {
				lua.set_function("OnKeyDown", [](const string& keyName) -> bool {
					return InputSystem::GetInstance()->OnKeyDown(keyName);
					});
			}
		}

		int result = luaL_loadstring(lua.lua_state(), codigo_lua.c_str());
		if (result == LUA_OK) {
			if (!LuaCompiled) {
				Object* OBJ = GetObject();
				lua["self"] = sol::make_object(lua.lua_state(), OBJ);
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