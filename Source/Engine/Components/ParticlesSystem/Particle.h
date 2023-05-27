#pragma once
#include "../Gizmos/GizmoDrawer.h"
#include "../Mathf/Vector2.h"
#include "../Object.h"
#include "../Mathf/GMathf.h"
#include <iostream>

using namespace std;

class Particle {
public:
	string TexturePath;
	SDL_Texture* texture;
	float DirectionX;
	float DirectionY;
	float PosX;
	float PosY;
	float DefaultTime = 200;
	float RotationXOffset;
	float RotationYOffset;
	float Speed;

	void Draw () {
		float DirX = DirectionX - RotationXOffset;
		float DirY = DirectionY - RotationYOffset;

		float magnitude = sqrt(DirX * DirX + DirY * DirY);

		if (magnitude != 0) {
			DirX /= magnitude;
			DirY /= magnitude;
		}

		b2Vec2 Position = b2Vec2(PosX, PosY) + b2Vec2(DirX * Speed, DirY * Speed);

		PosX = Position.x;
		PosY = Position.y;

		Vector2 Pos1 = Vector2(PosX, PosY);
		Vector2 Pos2 = Vector2(Position.x, Position.y);

		float GetDistance = Vector2::Distance(Pos1, Pos2);


		float centerX = PosX * 32;
		float centerY = PosY * 32;

		float textureX = centerX - 32 / 2 - Window::camera.x;
		float textureY = centerY - 32 / 2 - Window::camera.y;

		SDL_SetTextureAlphaMod(texture, 230);
		SDL_Rect dest_rect = { textureX, textureY, 32, 32 };
		SDL_RenderCopy(Window::renderer, texture, NULL, &dest_rect);
	}


	void Clear () {
		SDL_DestroyTexture(texture);
	}
};