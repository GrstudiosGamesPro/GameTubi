#include "Window.h"
#include "SDL.h"
#include "Components/Implementaciones/ImGuiImplement.h"
#include "Components/Object.h"
#include "Components/SceneManager/Scene.h"
#include "Components/SceneManager/ManagerScene.h"
#include "Components/AudioSource/AudioSource.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "SDL_image.h"


using namespace std;

SDL_Renderer* Window::renderer = nullptr;

SDL_Event Window::events;
SDL_Event event;

ImGuiImplement imgui = ImGuiImplement();

int Width, Height;
int Window::CAMERA_MAX_VEL = 25;
SDL_Surface* tempSurface;
SDL_Texture* tex;
SDL_Surface* tempSurface2;
SDL_Texture* tex2;
Object* CurrentObjectSelect;
bool done = false;
bool DragginObject = false;
SDL_Rect Window::camera = { 0, 0, Width, Height };
int cameraVelX = 0;
int cameraVelY = 0;
bool DragginMouse;
bool DragginMouseX;


Window::Window(){

}


void Window::StartWindow(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
#pragma region Start Engine
	int flags = 0;

	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		
		cout << "Sub sistema inicializado con exito..." << endl;

		if (window != nullptr) {
			cout << "Ventana inicializada..." << endl;
		}

	    renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer){
			SDL_SetRenderDrawColor (renderer, 0, 0, 0, 255);
			cout << "Render creado..." << endl;
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	imgui.SetupOnStart(window, renderer);
	imgui.wnd = this;



	ManagerScene::GetInstance()->GetCurrentScene()->OnStartScene();


	GridSize = 40;
	Width = width;
	Height = height;

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
		ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->Start();
	}

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Audio.size(); i++) {
		ManagerScene::GetInstance()->GetCurrentScene()->Audio[i]->Start();
	}
#pragma endregion
}

bool Window::running() {
	return isRunning;
}

void Window::handleEvents() {
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	case SDL_DROPFILE:
		Window::LoadFile (event.drop.file);
		break;

	default:
		break;
	}
}

void Window::LoadFile(string path) {
	std::filesystem::path patha(path);

	std::string source_path = path;
	std::string dest_path = "Assets/Sprites/" + patha.filename().string();

	std::ifstream source_file(source_path, std::ios::binary);

	// verificar si la apertura del archivo de origen fue exitosa
	if (!source_file.is_open()) {
		std::cerr << "No se pudo abrir el archivo de origen" << std::endl;
	}

	// abrir el archivo de destino en modo binario
	std::ofstream dest_file(dest_path, std::ios::binary);

	// verificar si la apertura del archivo de destino fue exitosa
	if (!dest_file.is_open()) {
		std::cerr << "No se pudo abrir el archivo de destino" << std::endl;
	}

	// determinar el tamaño del archivo de origen
	source_file.seekg(0, std::ios::end);
	std::streamsize size = source_file.tellg();
	source_file.seekg(0, std::ios::beg);

	// reservar un buffer de tamaño suficiente para el archivo
	std::vector<char> buffer(size);

	// leer todo el archivo en el buffer
	if (!source_file.read(buffer.data(), size)) {
		std::cerr << "No se pudo leer el archivo de origen" << std::endl;
	}

	// escribir todo el buffer en el archivo de destino
	if (!dest_file.write(buffer.data(), size)) {
		std::cerr << "No se pudo escribir el archivo de destino" << std::endl;
	}

	// cerrar los archivos
	source_file.close();
	dest_file.close();

	std::cout << "El archivo ha sido copiado correctamente" << std::endl;
}

void Window::OnUpdate() {
	count++;
	//Cuenta desde el primer frame
	//cout << "Frame: " << count << endl;

	handleEvents();
	OnRender();

	ManagerScene::GetInstance()->GetCurrentScene()->UpdateScene();

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
		ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->Update();
	}

	if (event.type == SDL_KEYDOWN) {
		if (event.button.button == SDL_SCANCODE_LCTRL) {
			DragginMouse = true;
		}
	}

	if (event.type == SDL_KEYUP) {
		if (event.button.button == SDL_SCANCODE_LCTRL) {
			DragginMouse = false;
		}
	}

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			DragginMouseX = true;
		}
	}

	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			DragginMouseX = false;
		}
	}


	int x = event.motion.x;
	int y = event.motion.y;

	int dx = event.motion.xrel;
	int dy = event.motion.yrel;

	int relX, relY;
	Uint32 mouseState = SDL_GetRelativeMouseState(&relX, &relY);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	float ppiX = static_cast<float>(w) / static_cast<float>(relX);
	float ppiY = static_cast<float>(h) / static_cast<float>(relY);

	if (DragginMouse && DragginMouseX) {
		if (dx >= CAMERA_MAX_VEL) {
			dx = CAMERA_MAX_VEL;
		}

		if (dx <= -CAMERA_MAX_VEL) {
			dx = -CAMERA_MAX_VEL;
		}

		if (dy >= CAMERA_MAX_VEL) {
			dy = CAMERA_MAX_VEL;
		}

		if (dy <= -CAMERA_MAX_VEL) {
			dy = -CAMERA_MAX_VEL;
		}


		if (relX != 0 && relY != 0) {
			camera.x -= dx;
			camera.y -= dy;
		}
	}

	SDL_GL_SwapWindow(window);
}


void Window::OnRender() {
	if (ManagerScene::GetInstance()->GetCurrentScene()->UseFullScreen) {
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);

		SDL_RenderCopy(Window::renderer, ManagerScene::GetInstance()->GetCurrentScene()->backgroundTexture, NULL, NULL);
	}
	else {
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
		SDL_Rect* BackgroundPosition = new SDL_Rect();

		BackgroundPosition->x -= camera.x;
		BackgroundPosition->y -= camera.y;
		BackgroundPosition->w = 100;
		BackgroundPosition->h = 100;
		SDL_RenderCopy(Window::renderer, ManagerScene::GetInstance()->GetCurrentScene()->backgroundTexture, NULL, BackgroundPosition);
	}

#pragma region Render IMGUI AND OBJECTS


	const int grd = GridSize;

	if (GridSize < 6) {
		GridSize = 5;
	}
	else {
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		for (int y = 0; y < Height; y += grd)
		{
			SDL_RenderDrawLine(renderer, 0, y, Width, y);
		}

		// Draw vertical lines
		for (int x = 0; x < Width; x += grd)
		{
			SDL_RenderDrawLine(renderer, x, 0, x, Height);
		}

	}

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
		ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->Draw();
	}

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Audio.size(); i++) {
		ManagerScene::GetInstance()->GetCurrentScene()->Audio[i]->Draw ();
	}

	imgui.RenderUI (event);

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			imgui.MousePosition.x = mouseX;
			imgui.MousePosition.y = mouseY;

			imgui.ViewPortMenu = true;
		}
	}

	int mouseX = 0; // Mover declaración e inicialización aquí
	int mouseY = 0;

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			DragginObject = true;
		}
	}

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			CurrentObjectSelect = nullptr;
			DragginObject = false;
		}
	}

	if (!DragginObject) {
		CurrentObjectSelect = nullptr;
	}

	if (DragginObject) {
		for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
			Object* obj = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];
			SDL_GetMouseState(&mouseX, &mouseY);

			SDL_Rect Dest = *obj->GetRectDEST();

			if (CurrentObjectSelect == nullptr && mouseX >= obj->GetPosition().x && mouseX <= obj->GetPosition().x + Dest.w && mouseY >= obj->GetPosition().y && mouseY <= obj->GetPosition().y + Dest.h)
			{
				Object* obj2 = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];
				CurrentObjectSelect = obj2;
			}
		}

		if (CurrentObjectSelect != nullptr) {
			Object* obj2 = CurrentObjectSelect;
			SDL_QueryTexture(obj2->GetTexture(), NULL, NULL, &obj2->GetRectDEST()->w, &obj2->GetRectDEST()->h);

			float distancia = sqrt(pow(mouseX - obj2->pos.x, 2) + pow(mouseY - obj2->pos.y, 2));

			int distanceX = obj2->pos.x;
			int distanceY = obj2->pos.y;


			//obj2->SetPosition (distanceX, distanceY);
		}
	}


	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			imgui.ViewPortMenu = false;
		}
	}
#pragma endregion

	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
	/*
		int x = newPlayer.getComponent<Transform>().position.x;
		int y = newPlayer.getComponent<Transform>().position.y;
		int side = 100;
		int thickness = 5;

		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_Rect rect = { x, y, side, side };
		//SDL_RenderFillRect(renderer, &rect);
		rect.x -= thickness;
		rect.y -= thickness;
		rect.w += 2 * thickness;
		rect.h += 2 * thickness;
		SDL_RenderDrawRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);

		int mouseX, mouseY = 0;
		Uint32 mouse_state = SDL_GetMouseState(&mouseX, &mouseY);

		//printf("Mouse position: (%d, %d)\n", mouseX, mouseY);

		int asdX = newPlayer.getComponent<Transform>().position.x;
		int asdY = newPlayer.getComponent<Transform>().position.y;
		int side2 = 100;
		int thickness2 = 5;
		int mouse_x, mouse_y;

		SDL_Rect rect2 = { asdX, asdY, side2, side2 };

		if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if (mouse_x >= rect2.x && mouse_x <= rect2.x + rect2.w &&
				mouse_y >= rect2.y && mouse_y <= rect2.y + rect2.h) {

				newPlayer.getComponent <Transform>().position.x = mouseX;
				newPlayer.getComponent <Transform>().position.y = mouseY;
				printf("Clicked object with index 0\n");
			}
		}
	*/

	/*

	SDL_Rect renderRect = { 0, 0, Width, Height };
	SDL_RenderSetClipRect(renderer, &renderRect);

	// Establecer la distancia de renderizado de la cámara
	int cameraFar = 10; // Aumentar este valor para aumentar la distancia de renderizado
	SDL_Rect cameraRect = { cameraX - Width / 2, cameraY - Height / 2, Width, Height };
	cameraRect.x -= cameraFar;
	cameraRect.y -= cameraFar;
	cameraRect.w += cameraFar * 2;
	cameraRect.h += cameraFar * 2;

	SDL_RenderSetClipRect (renderer, &cameraRect);

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
		if (ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->x > cameraX - Width / 2 - cameraFar &&
			ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->x < cameraX + Width / 2 + cameraFar &&
			ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->y > cameraY - Height / 2 - cameraFar &&
			ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->y < cameraY + Height / 2 + cameraFar) {
			SDL_Rect objectRect = { ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->x - ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->w / 2 - cameraX + Width / 2, ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->y - ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->h / 2 - cameraY + Height / 2, ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->w, ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetRectDEST()->h};
			//SDL_RenderCopy(renderer, ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->GetTexture(), nullptr, &objectRect);
		}
	}
	SDL_RenderSetClipRect(renderer, nullptr);

	*/

	SDL_Rect screenRect = { 0, 0, Width, Height };
	SDL_Rect cameraRect = { camera.x, camera.y, camera.w, camera.h };

	SDL_RenderSetClipRect(renderer, &screenRect);
	imgui.GetRenderDrawData();
	SDL_RenderPresent	(renderer);
}



void Window::OnClean() {
	imgui.OnDestroy();
	SDL_DestroyWindow (window);
	SDL_DestroyRenderer (renderer);
	SDL_Quit();
	cout << "Engine limpiado..." << endl;
}

Window::~Window() {

}