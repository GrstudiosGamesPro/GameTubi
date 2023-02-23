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
		SDL_SetTextureAlphaMod(texture, 128);
		SDL_Rect dest_rect = { posX, posY, 32, 32 };
		SDL_RenderCopy(Window::renderer, texture, NULL, &dest_rect);
	}
};