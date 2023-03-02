//HOLA
#include "Object.h"
#include "box2d.h"
#include "SceneManager/ManagerScene.h"
#include "SaveSystem/SaveData.h"
#include "Object/ObjectManager.h"

static ObjectManager* OBJManager;

Object::Object () {
	std::cout << "Objeto creado" << endl;
}

 
void Object::Start() {
	text = TextureManager::LoadTexture("Assets/Sprites/idle.gif");
	TexturePath = "Assets/Sprites/idle.gif";
	srcRect.x = srcRect.y = 0;

	srcRect.w = width;
	srcRect.h = height;
	destRect.w = width * ScaleX;
	destRect.h = height * ScaleY;
	Object::CreateBody();
	body->SetTransform(b2Vec2(pos.x, pos.y), 0);
	OBJManager = new ObjectManager();
	source = new AudioSource();
	source->Start();
}

void Object::Update() {
	if (isActive) {
		InputSystem();		
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

		destRect.x = textureX - Window::camera.x;
		destRect.y = textureY - Window::camera.y;

		destRect.w =  width  * ScaleX;
		destRect.h =  height * ScaleY;

		pos.x = body->GetPosition().x;
		pos.y = body->GetPosition().y;
		Angle = body->GetAngle();


		if (useGravity && body != nullptr) 
		{
			body->SetAwake (true);
			body->SetType(b2_dynamicBody);
		}
		else {
			body->SetType (b2_staticBody);
		}

		if (LuaCompiled) {
			lua["Tick"]();
		}
	}
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
		body = ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->CreateBody (&bodyDef);

		b2Vec2 localCenter(0.0f, 0.0f);
		localCenter.Set(0, 0);
		Object::UpdateCollisions();
		fixtureDef->shape = dynamicBox;
		body->CreateFixture(fixtureDef);
	}
}


void Object::UpdateCollisions() {

	float textureCenterX = width / 2.0f * ScaleBoxX;
	float textureCenterY = height / 2.0f * ScaleBoxY;
	b2Vec2 localCenter (0.0f, 0.0f);
	localCenter.Set (0, 0);

	int texWidth = width;
	int texHeight = height;

	// Factor de escala
	float scaleX = (float)ScaleBoxX;
	float scaleY = (float)ScaleBoxY;
	fixtureDef->density = density;
	fixtureDef->friction = friction;
	// Tamaño de la caja de colisión
	float boxWidth = (float)texWidth * scaleX / (float)width / 2.0f;
	float boxHeight = (float)texHeight * scaleY / (float)height / 2.0f;

	body->DestroyFixture(body->GetFixtureList());
	dynamicBox->SetAsBox (float(width) * boxWidth / width, float(height) * boxHeight / height, localCenter, 0);
	fixtureDef->shape = dynamicBox;
	body->CreateFixture(fixtureDef);	
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
		TextureManager::Draw(text, srcRect, Angle, destRect);

		float angle = body->GetAngle();
		b2PolygonShape* shape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
		b2Vec2 vertices[b2_maxPolygonVertices];

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
		SDL_SetRenderDrawColor(Window::renderer, 255, 0, 0, 255);
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


void Object::InputSystem() {
	
}

Object::~Object() {

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
	std::cout << "Aplicando fuerza a " << name << endl;
}

Object* Object::GetObject() {
	return this;
}


void Object::CallLua(string ScriptToRun) {
	std::string codigo_lua = ScriptToRun;

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


	lua.new_usertype<Vector2>("Vector2", sol::constructors<Vector2(float, float)>(),
		"x", &Vector2::x,
		"y", &Vector2::y);

	lua["Vector2"]["new"] = [](float x, float y) {
		return Vector2(x, y);
	};

	lua["self"] = sol::make_object(lua.lua_state(), this);
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
		"SetPosition", &AudioSource::SetPosition
	);

	lua["AudioSource"]["FindSourcePerName"] = [](string FindName) {return OBJManager->FindAudioSourcePerName(FindName); };
	lua["AudioSource"] = source;

	int result = luaL_loadstring(lua.lua_state(), ScriptToRun.c_str());

	if (result == LUA_OK) {
		result = lua_pcall(lua.lua_state(), 0, LUA_MULTRET, 0);
		if (result != LUA_OK) {
			std::string error = lua_tostring(lua.lua_state(), -1);
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
		lua_pop(lua.lua_state(), 1);
		std::cout << "Lua error on compiled " << endl;
	}
}