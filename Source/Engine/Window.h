#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <iostream>
#include "Components/TextureManager.h"
#include <iostream>

class Window
{
public:
	Window();
	~Window();

	void StartWindow (const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	//Eventos
	void handleEvents();
	void OnUpdate();
	void OnRender();
	void OnClean();

	bool running();

	
	static SDL_Renderer* renderer;
	static SDL_Event events;
	static SDL_Rect camera;
	static float Sensibilidad;
	int GridSize;

private:
	int count;
	bool isRunning;
	SDL_Window* window;
};