#pragma once
#define 
#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <iostream>
#include "Components/TextureManager.h"
#include <iostream>

using namespace std;

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
	static int CAMERA_MAX_VEL;
	void LoadFile (string path);

private:
	int count;
	bool isRunning;
	SDL_Window* window;
};