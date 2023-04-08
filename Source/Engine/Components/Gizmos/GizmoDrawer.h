#pragma once
#include <iostream>
#include "SDL.h"
#include <SDL_image.h>
#include "../../Window.h"

using namespace std;

class GizmoDrawer
{
public:
	string TexturePath;
	SDL_Texture* texture;

	void Start (string TextureName) {
		string tt = "Assets/Gizmos/" + TextureName + ".png";
		TexturePath = tt;
		SDL_Surface* image = IMG_Load(tt.c_str());
		texture = SDL_CreateTextureFromSurface(Window::renderer, image);
		SDL_FreeSurface(image);
	}

	void Draw (float posX, float posY) {

		float centerX = posX * 32;
		float centerY = posY * 32;

		float textureX = centerX - 32 / 2 - Window::camera.x;
		float textureY = centerY - 32 / 2 - Window::camera.y;

		SDL_SetTextureAlphaMod(texture, 230);
		SDL_Rect dest_rect = { textureX, textureY, 32, 32 };
		SDL_RenderCopy(Window::renderer, texture, NULL, &dest_rect);
	}

	~GizmoDrawer() {
		SDL_DestroyTexture(texture);
	}
};