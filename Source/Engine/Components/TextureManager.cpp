#include "TextureManager.h"


SDL_Texture* TextureManager::LoadTexture (const char* texture) {
	SDL_Surface* tempSurface = IMG_Load (texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface (Window::renderer, tempSurface);
	SDL_FreeSurface (tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, float Angle , SDL_Rect dest) {
	SDL_RenderCopyEx (Window::renderer, tex, &src, &dest, Angle, NULL, SDL_FLIP_NONE);
}