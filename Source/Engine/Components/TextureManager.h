#pragma once
#include "../Window.h"

class TextureManager {

public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static void Draw (SDL_Texture* tex, SDL_Rect src, float Angle, SDL_Rect dest);
};