#include "Object.h"
#include "box2d.h"
#include "SceneManager/ManagerScene.h"


Object::Object() {
	
}

 
void Object::Start() {
	text = TextureManager::LoadTexture("Assets/Sprites/landingoutline.png");
	TexturePath = "Assets/Sprites/landingoutline.png";


	srcRect.x = srcRect.y = 0;

	srcRect.w = width;
	srcRect.h = height;
	destRect.w = width * ScaleX;
	destRect.h = height * ScaleY;

	if (text != nullptr) {
		std::cout << "Textura cargada con exito: " << endl;
	}

	CreateBody();
}

void Object::Update() {
	if (isActive) {
		InputSystem();

		pos.x = body->GetPosition().x;
		pos.y = body->GetPosition().y;

		
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

		destRect.x = textureX;
		destRect.y = textureY;

		destRect.w =  width  * ScaleX;
		destRect.h =  height * ScaleY;

		body->SetTransform(b2Vec2(pos.x, pos.y), 0);

		if (useGravity && body != nullptr) 
		{
			body->SetType(b2_dynamicBody);
		}
		else {
			body->SetType (b2_staticBody);
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

	// Tamaño de la caja de colisión
	float boxWidth = (float)texWidth * scaleX / (float)width / 2.0f;
	float boxHeight = (float)texHeight * scaleY / (float)height / 2.0f;

	body->DestroyFixture(body->GetFixtureList());
	printf("%g %g %g %g\n", float(width), float(ScaleBoxX), float(height), float(ScaleBoxY));
	dynamicBox->SetAsBox (float(width) * boxWidth / width, float(height) * boxHeight / height, localCenter, 0);
	fixtureDef->shape = dynamicBox;
	body->CreateFixture(fixtureDef);	
	ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->Dump();
}


string Object::GetName() {
	return name;
}

void Object::SetName (string _name) {
	name = _name;

}


void Object::SetPosition (float x, float y) {
	if (body != nullptr) {
		body->SetTransform(b2Vec2(x, y), 0);
	}
}

Vector2 Object::GetPosition() {
	
	if (body != nullptr) {
		return Vector2(body->GetPosition().x, body->GetPosition().y);
	}

	return Vector2(body->GetPosition().x, body->GetPosition().y);
}

void Object::Draw() {
	if (isActive) {
		TextureManager::Draw(text, srcRect, Angle, destRect);
		

		b2Vec2 position = body->GetPosition();
		float angle = body->GetAngle();
		b2PolygonShape* shape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
		b2Vec2 vertices[b2_maxPolygonVertices];

		for (int i = 0; i < shape->m_count; i++) {
			vertices[i] = body->GetWorldPoint(shape->m_vertices[i]);
		}

		// Convertir los vértices de Box2D a píxeles
		int pixelVertices[b2_maxPolygonVertices * 2];
		for (int i = 0; i < shape->m_count; i++) {
			pixelVertices[i * 2] = (int)(vertices[i].x * 32);
			pixelVertices[i * 2 + 1] = (int)(vertices[i].y * 32);
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
	TexturePath = TexturePath.c_str();
}

Object::~Object() {
	lua_close (m_PTRLuaState);
	SDL_DestroyTexture (text);
}

void Object::InputSystem() {
	
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

void Object::CompileLua() {
	m_PTRLuaState = luaL_newstate();

	if (m_PTRLuaState) {
		luaL_openlibs (m_PTRLuaState);

		char SCRP[512];

		strcpy_s (SCRP, Script.c_str());

		if (luaL_loadstring (m_PTRLuaState, SCRP) != LUA_OK) {
			std::cout << "Error on compile lua: " << lua_tostring (m_PTRLuaState, -1) << endl;
		}
		else {
			if (lua_pcall (m_PTRLuaState, 0, 0, 0) != LUA_OK) {

			}

			if (lua_getglobal(m_PTRLuaState, "OnStart") == LUA_TFUNCTION) {
				if (lua_pcall(m_PTRLuaState, 0, 1, 0) == LUA_OK) {

				}
			}

			if (lua_getglobal(m_PTRLuaState, "OnUpdate") == LUA_TFUNCTION) {
				if (lua_pcall(m_PTRLuaState, 0, 1, 0) == LUA_OK) {

				}
			}
		}
	}
}