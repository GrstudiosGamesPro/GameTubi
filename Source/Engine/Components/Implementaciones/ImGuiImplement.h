#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "../SceneManager/ManagerScene.h"
#include <iostream>
#include <cstring>
#include <tchar.h>
#include "../Object.h"
#include "../InputSystem/InputSystem.h"

using namespace std;

class ImGuiImplement {
public:
	Window* wnd;
	SDL_Texture* texture;
	Object* SelectObject;

	void SetupOnStart(SDL_Window* window, SDL_Renderer* renderer) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer_Init(renderer);

	}

	void RenderUI(SDL_Event event) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();

		ImGui::NewFrame();

		DrawUI();


		ImGui::Render();
	}

	void GetRenderDrawData() {
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}

	void OnDestroy() {
		ImGui_ImplSDLRenderer_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

public:
	bool ViewWorldSettings;
	bool ViewEngineSettings;
	bool ViewPortMenu;
	bool CodeEditor;
	bool InputSystem;
	ImVec2 MousePosition;

private:

	void DrawUI() {


		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save")) {

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings")) {
				if (ImGui::MenuItem("Editor")) {
					ViewEngineSettings = true;
				}

				if (ImGui::MenuItem("Input Manager")) {
					InputSystem = true;
				}

				if (ImGui::MenuItem("World Settings")) {
					ViewWorldSettings = true;
				}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}





		if (ImGui::Begin("Hierarchy")) {

			for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
				ImGui::PushID(i);

				if (!ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->isActive) {
					string InactiveName = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->name + " (Inactive)";

					if (ImGui::Button(InactiveName.c_str())) {
						if (SelectObject == ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]) {
							SelectObject = nullptr;
						}
						else {
							SelectObject = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];
						}
					}
				}
				else {
					if (ImGui::Button(ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->name.c_str())) {
						if (SelectObject == ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]) {
							SelectObject = nullptr;
						}
						else {
							SelectObject = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];
						}
					}
				}
				ImGui::PopID();
			}

			ImGui::End();
		}



		if (SelectObject != nullptr) {
			if (ImGui::Begin("Inspector")) {
				ImGui::PushID(34233);

				bool IsActive = SelectObject->isActive;
				ImGui::Checkbox("Is Active", &IsActive);
				SelectObject->isActive = IsActive;

				//NOMBRE
				char name[128];
				strcpy_s(name, SelectObject->GetName().c_str());
				ImGui::InputText("Name: ", name, ImGuiInputTextFlags_AutoSelectAll);
				SelectObject->SetName((std::string)name);

				float GetPosSD[2]{
					SelectObject->GetPosition().x,
					SelectObject->GetPosition().y,
				};

				ImGui::DragFloat2("Position: ", GetPosSD, 0.01f);

				if (SelectObject->pos.x != GetPosSD[0] || SelectObject->pos.y != GetPosSD[1]) {
					//SelectObject->UpdateCollisions();
				}

				SelectObject->SetPosition (GetPosSD[0], GetPosSD[1]);


				float ScaleX = SelectObject->ScaleX;
				ImGui::DragFloat("Scale X: ", &ScaleX, 0.01f);
				if (SelectObject->ScaleX != ScaleX) {
					SelectObject->UpdateCollisions();
				}

				SelectObject->ScaleX = ScaleX;

				float ScaleY = SelectObject->ScaleY;
				ImGui::DragFloat("Scale Y: ", &ScaleY, 0.01f);
				if (SelectObject->ScaleY != ScaleY) {
					SelectObject->UpdateCollisions();
				}
				SelectObject->ScaleY = ScaleY;




				float AngleM = SelectObject->Angle;
				ImGui::DragFloat("Angle: ", &AngleM);
				SelectObject->Angle = AngleM;

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				char TexturePath[100];
				strcpy_s(TexturePath, SelectObject->TexturePath.c_str());
				ImGui::InputText("Texture: ", TexturePath, 100, ImGuiInputTextFlags_AutoSelectAll);

				ImGui::Image(TextureManager::LoadTexture(TexturePath), ImVec2(128, 128));
				SelectObject->TexturePath = (std::string)TexturePath;

				if (ImGui::Button("Save Sprites")) {
					SelectObject->TexturePath = TexturePath;
					SelectObject->SetNewTexture();
				}
				ImGui::PopID();

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::PushID (3423);
				ImGui::Text ("Collisions");
				bool UseCollisions = SelectObject->useGravity;
				ImGui::Checkbox ("Active: ", &UseCollisions);
				SelectObject->useGravity = UseCollisions;
				ImGui::Spacing();


				float ScaleBoxX = SelectObject->ScaleBoxX;
				ImGui::DragFloat("Scale X: ", &ScaleBoxX, 0.01f);
				if (SelectObject->ScaleX != ScaleBoxX) {
					SelectObject->UpdateCollisions();
				}

				SelectObject->ScaleBoxX = ScaleBoxX;

				float ScaleBoxY = SelectObject->ScaleBoxY;
				ImGui::DragFloat("Scale Y: ", &ScaleBoxY, 0.01f);
				if (SelectObject->ScaleBoxY != ScaleBoxY) {
					SelectObject->UpdateCollisions();
				}
				SelectObject->ScaleBoxY = ScaleBoxY;


				float Density = SelectObject->density;
				ImGui::DragFloat ("Density: ", &Density, 0.01f);
				SelectObject->density = Density;

				float Friction = SelectObject->friction;
				ImGui::DragFloat("Friction: ", &Friction, 0.01f);
				SelectObject->friction = Friction;
				ImGui::PopID();

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				if (ImGui::Button("Script Editor")) {
					CodeEditor = true;
				}

				ImGui::End();
			}
		}

		ImGui::PushID (12321);
		if (ViewEngineSettings) {
			if (ImGui::Begin("Editor Settings")) {
				int GetGridSize = wnd->GridSize;
				ImGui::DragInt("Grid Size: ", &GetGridSize);
				wnd->GridSize = GetGridSize;


				if (ImGui::Button("Close")) {
					ViewEngineSettings = false;
				}
				ImGui::End();
			}
		}
		ImGui::PopID();





		ImGui::PushID (6433);

		if (ViewWorldSettings) {
			if (ImGui::Begin ("World Settings")) {
				b2Vec2 Gravity = ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->GetGravity();

				float GravityScaleGet[2]{
					ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->GetGravity().x,
					ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->GetGravity().y
				};

				ImGui::DragFloat2("Gravity Scale: ", GravityScaleGet);
				ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->SetGravity(b2Vec2(GravityScaleGet[0], GravityScaleGet[1]));

				if (ImGui::Button("Close")) {
					ViewWorldSettings = false;
				}

				ImGui::End();
			}
		}

		ImGui::PopID();

		if (ViewPortMenu) {
			ImGui::SetNextWindowPos(ImVec2(MousePosition.x, MousePosition.y));
			ImGui::SetNextWindowSize(ImVec2(300, 500));

			if (ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

				if (ImGui::BeginMenu("Create")) {
					if (ImGui::MenuItem("Game Object")) {
						ManagerScene::GetInstance()->GetCurrentScene()->SetupNewObject();
					}
					ImGui::EndMenu();
				}

				ImGui::End();
			}
		}

		if (CodeEditor) {
			char str_hold[2048] = "";



			ImGuiInputTextFlags flags = ImGuiInputTextFlags_CtrlEnterForNewLine;

			ImGui::SetNextWindowSize(ImVec2(800, 650));
			if (ImGui::Begin("Script Editor", NULL, ImGuiWindowFlags_NoResize)) {
				ImGui::Text("Press (CTRL + Enter) for jump line");

				ImVec2 Size;
				Size.x = 800;
				Size.y = 600;

				strcpy_s (str_hold, SelectObject->Script.c_str());

				ImGui::InputTextMultiline(" ", str_hold, 999999, Size, flags);
				SelectObject->Script = (string)str_hold;
				if (ImGui::Button("Compile")) {
					SelectObject->CompileLua();
				}
				ImGui::End();
			}

			
		}

		if (InputSystem) {

			if (ImGui::Begin("Input Manager")) {
				if (ImGui::Button("Create Axis")) {
					InputSystem::GetInstance()->SetNewInput("Test", "w", "1", "-1");
				}

				for (int i = 0; i < InputSystem::GetInstance()->inputs.size(); i++) {

					ImGui::PushID(i);
					char _Name[128];
					strcpy_s(_Name, InputSystem::GetInstance()->inputs[i].Name.c_str());
					ImGui::InputText("Name: ", _Name, ImGuiInputTextFlags_AutoSelectAll);
					InputSystem::GetInstance()->inputs[i].Name = (string)_Name;

					char KeyName[128];
					strcpy_s(KeyName, InputSystem::GetInstance()->inputs[i].Key.c_str());
					ImGui::InputText("Key: ", KeyName, ImGuiInputTextFlags_AutoSelectAll);
					InputSystem::GetInstance()->inputs[i].Key = (string)KeyName;

					char KeyAxis[128];
					strcpy_s(KeyAxis, InputSystem::GetInstance()->inputs[i].Axis.c_str());
					ImGui::InputText("Axis: VALUE +", KeyAxis, ImGuiInputTextFlags_AutoSelectAll);
					InputSystem::GetInstance()->inputs[i].Axis = (string)KeyAxis;

					char NegateKeyAxis[128];
					strcpy_s(NegateKeyAxis, InputSystem::GetInstance()->inputs[i].NegateAxis.c_str());
					ImGui::InputText("Negate Axis: VALUE -", NegateKeyAxis, ImGuiInputTextFlags_AutoSelectAll);
					InputSystem::GetInstance()->inputs[i].NegateAxis = (string)NegateKeyAxis;			

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::PopID();
				}
			}

			ImGui::End();
		}
	}
};