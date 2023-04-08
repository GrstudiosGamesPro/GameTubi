#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "../SceneManager/ManagerScene.h"
#include "../../Window.h"
#include <iostream>
#include "../Console/ConsoleManager.h"
#include <cstring>
#include <tchar.h>
#include "../Object.h"
#include "../InputSystem/InputSystem.h"
#include "../../WindowsCompiler/WindowsCompiler.h"
#include "../AudioSource/AudioSource.h"
#include "../ParticlesSystem/ParticlesSystem.h"
#include "../Animator/AnimatorSprite.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace std;

class ImGuiImplement {
public:
	Window* wnd;
	Object* SelectObject;
	AudioSource* CurrentAudioSourceSelect;
	ParticlesSystem* CurrentParticleSelect;
	AnimatorSprite* animator = new AnimatorSprite();
	SDL_Texture* CurrentParticleTexture;

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
	bool BuildSettings;
	bool AnimatorWindows;
	ImVec2 MousePosition;

private:

	void DrawUI() {

		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		ImGui::GetStyle().Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
		ImGui::GetStyle().Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
		ImGui::GetStyle().Colors[ImGuiCol_NavHighlight] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

		ImGui::PushStyleVar		(ImGuiStyleVar_WindowRounding, 10.0f);
		ImGui::PushStyleVar		(ImGuiStyleVar_FrameRounding, 10.0f);
		ImGui::PushStyleVar		(ImGuiStyleVar_FrameRounding, 10.0f);
		ImGui::PushStyleColor	(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor   (ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor	(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Scene")) {
					ManagerScene::GetInstance()->GetCurrentScene()->LoadScene("NewScene");
				}

				if (ImGui::MenuItem("Save")) {
					ManagerScene::GetInstance()->GetCurrentScene()->SaveScene();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Build")) {
				if (ImGui::MenuItem("Build Settings")) {
					BuildSettings = true;
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

				if (ImGui::MenuItem("Animator Editor")) {
					AnimatorWindows = true;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}


		if (AnimatorWindows) {
			ImGui::SetNextWindowSize(ImVec2(1000, 700));

			if (ImGui::Begin("Animator", NULL, ImGuiWindowFlags_NoResize | ImGuiInputTextFlags_EnterReturnsTrue)) {

				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);

				ImGui::SetCursorPos(ImVec2(975, 20));
				if (ImGui::Button("X", ImVec2(20, 20))) {
					AnimatorWindows = false;
				}

				if (ImGui::BeginMenu("Animations")) {
					for (int i = 0; i < animator->Sprites.size(); i++) {
						ImGui::PushID(i);
						string TestTXT = "Assets/Sprites/idle.gif";
						if (ImGui::ImageButton(TextureManager::LoadTexture(TestTXT.c_str()), ImVec2(72, 72))) {

						}
						ImGui::PopID();
					}
					ImGui::EndMenu();
				}


				string TestTXT = "Assets/Sprites/idle.gif";
				ImGui::Image(TextureManager::LoadTexture(TestTXT.c_str()), ImVec2(512, 512));



				ImGui::SetCursorPos(ImVec2(900, 670));
				if (ImGui::Button("New Sprite", ImVec2(80, 20))) {
					animator->SetupNewTexture("XX");
				}

				ImGui::End();
			}
		}


		ImGui::PushID(252334);

		if (BuildSettings) {
			if (ImGui::Begin("Build Settings")) {
				ImGui::SetNextWindowSize(ImVec2(800, 650));
				if (ImGui::Button("Close")) {
					BuildSettings = false;
				}

				if (ImGui::BeginChild("Scenes")) {

					std::filesystem::path DirectoryPath = "Assets/SaveData";
					std::string extensionRequire = ".Scene";

					std::filesystem::directory_iterator begin(DirectoryPath);
					std::filesystem::directory_iterator end;
					ImGui::Text ("Scene Build");

					ImGui::Spacing();

					for (std::filesystem::directory_iterator iter = begin; iter != end; ++iter) {
						if (iter->path().extension() == extensionRequire) {
							std::string WithoutExtension = iter->path().stem().string();
							string getPath = "Assets/SaveData/" + WithoutExtension + extensionRequire;

							std::ifstream file(getPath.c_str());

							if (file) {
								json data;
								file >> data;

								bool Active = (bool)data["CompileScene"];
								string GetPath = WithoutExtension;

								if (ImGui::Checkbox(GetPath.c_str(), &Active)) {
									std::cout << "Cambiando Estado: " << WithoutExtension << endl;
									bool NewValueCompiler = !(bool)data["CompileScene"];
									data["CompileScene"] = NewValueCompiler;
									ManagerScene::GetInstance()->GetCurrentScene()->CompileScene = (bool)data["CompileScene"];
									std::ofstream outfile("Assets/SaveData/" + WithoutExtension + extensionRequire);
									outfile << data.dump(4);
								}
							}
						}
					}

					ImGui::EndChild();
				}

				if (ImGui::Button("Build")) {
					WindowsCompiler CompileData = WindowsCompiler();
					CompileData.CompileGame();
				}
				ImGui::Spacing ();
				ImGui::Spacing ();
				ImGui::Spacing ();
				ImGui::End();
			}
		}

		ImGui::PopID();

		string GetSceneName = ManagerScene::GetInstance()->GetCurrentScene()->SceneName;

		if (ImGui::Begin("Hierarchy")) {

			const char* Title = " ";

			ImGui::Text (GetSceneName.c_str());

			ImGui::Spacing ();

			/*
			if (ImGui::TreeNode("Workspace")) {
				for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
					ImGui::PushID(i);

					bool is_inactive = !ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->isActive;
					std::string button_name = is_inactive ? ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->name + " (Inactive)" : ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->name;

					if (ImGui::Selectable(button_name.c_str(), false, ImGuiSelectableFlags_AllowItemOverlap)) {
						if (SelectObject == ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]) {
							SelectObject = nullptr;
						}
						else {
							SelectObject = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];
						}
					}
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
						ImGui::SetDragDropPayload("SELECTABLE", &i, sizeof(int));
						ImGui::Text(button_name.c_str());
						ImGui::EndDragDropSource();
					}
					
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			*/

			
			
			if (ImGui::TreeNode("Workspace")) {
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
					if (ImGui::Button("Delete")) {
						ManagerScene::GetInstance()->GetCurrentScene()->DestroyObject(ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]);
					}
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			


			if (ImGui::TreeNode("Audio Source")) {

				for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Audio.size(); i++) {
					ImGui::PushID(i);
					if (ImGui::Button(ManagerScene::GetInstance()->GetCurrentScene()->Audio[i]->Name.c_str())) {
						if (CurrentAudioSourceSelect == ManagerScene::GetInstance()->GetCurrentScene()->Audio[i]) {
							CurrentAudioSourceSelect = nullptr;
						}
						else {
							CurrentAudioSourceSelect = ManagerScene::GetInstance()->GetCurrentScene()->Audio[i];
						}
					}
					ImGui::PopID();
				}

				ImGui::TreePop();
			}


			if (ImGui::TreeNode("Particles System")) {

				for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Particles.size(); i++) {
					ImGui::PushID(i);
					if (ImGui::Button(ManagerScene::GetInstance()->GetCurrentScene()->Particles[i]->Name.c_str())) {
						if (CurrentParticleSelect == ManagerScene::GetInstance()->GetCurrentScene()->Particles[i]) {
							CurrentParticleSelect = nullptr;
						}
						else {
							if (CurrentParticleTexture == nullptr) {
								SDL_DestroyTexture (CurrentParticleTexture);
							}

							CurrentParticleTexture = TextureManager::LoadTexture(ManagerScene::GetInstance()->GetCurrentScene()->Particles[i]->ParticlePath.c_str());
							CurrentParticleSelect = ManagerScene::GetInstance()->GetCurrentScene()->Particles[i];
						}
					}
					ImGui::PopID();
				}

				ImGui::TreePop();
			}
			ImGui::End();
		}

		
		if (SelectObject != nullptr) {
			if (ImGui::Begin("Inspector")) {
				if (ImGui::Button("Close"))
				{
					SelectObject = nullptr;
				}
				else {
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::PushID(34233);


					ImGui::BeginGroup();
					bool Estatico = SelectObject->IsStatic;
					ImGui::Checkbox("Static", &Estatico);
					SelectObject->IsStatic = Estatico;

					bool IsActive = SelectObject->isActive;
					ImGui::Checkbox("Active", &IsActive);
					SelectObject->isActive = IsActive;
					ImGui::EndGroup();

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

					SelectObject->SetPosition(GetPosSD[0], GetPosSD[1]);


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

					char TexturePath[100];
					strcpy_s(TexturePath, SelectObject->TexturePath.c_str());
					ImGui::InputText("Texture: ", TexturePath, 100, ImGuiInputTextFlags_AutoSelectAll);
					SelectObject->TexturePath = TexturePath;

					ImGui::PushID(241);
					ImGui::Image(SelectObject->text, ImVec2(128, 128));
					ImGui::PopID();
					



					if (ImGui::Button("Save Sprites")) {
						SelectObject->SetNewTexture();
					}

					ImGui::PopID();

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::PushID(3423);
					ImGui::Text("Collisions");
					bool UseCollisions = SelectObject->useGravity;
					ImGui::Checkbox("Gravity: ", &UseCollisions);
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
					ImGui::DragFloat("Density: ", &Density, 0.01f);
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
				}
				ImGui::End();
			}
		}



		if (CurrentParticleSelect != nullptr) {
			if (ImGui::Begin("Particle Setting")) {
				char name[128];
				strcpy_s(name, CurrentParticleSelect->Name.c_str());
				ImGui::InputText("Particle Name", name, ImGuiInputTextFlags_AutoSelectAll);
				CurrentParticleSelect->Name = (string)name;

				float GetPosSD[2]{
					CurrentParticleSelect->Position.x,
					CurrentParticleSelect->Position.y,
				};

				int RadiusSize = CurrentParticleSelect->LifeTime;
				int MaxInstances = CurrentParticleSelect->MaxParticles;

				ImGui::DragFloat2	("Position: ", GetPosSD, 0.01f);
				ImGui::DragInt		("Life Time: ", &RadiusSize, 1, 0, 99999);
				ImGui::DragInt		("Max Instances: ", &MaxInstances, 1, 0, 99999);
				CurrentParticleSelect->LifeTime = RadiusSize;
				CurrentParticleSelect->MaxParticles = MaxInstances;


				float SpawnTime = CurrentParticleSelect->SpawnTime;
				ImGui::DragFloat ("Spawn time: ", &SpawnTime, 1, 0, 9999);
				CurrentParticleSelect->SpawnTime = SpawnTime;

				float Speed = CurrentParticleSelect->Speed;
				ImGui::DragFloat ("Move Speed: ", &Speed, 0.001, -9999, 9999);
				CurrentParticleSelect->Speed = Speed;

				ImGui::Spacing();
				ImGui::Spacing();


				char TexturePath2[100];
				strcpy_s(TexturePath2, CurrentParticleSelect->ParticlePath.c_str());
				ImGui::InputText("Texture: ", TexturePath2, 100, ImGuiInputTextFlags_AutoSelectAll);

				if (CurrentParticleTexture != nullptr) {
					ImGui::PushID(2441);
					ImGui::Image(CurrentParticleTexture, ImVec2(128, 128));
					ImGui::PopID();
				}


				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				float MinDirection = CurrentParticleSelect->MinxRotationDir;
				ImGui::DragFloat ("Min Direction: ", &MinDirection, 1, -360, CurrentParticleSelect->MaxRotationDir);
				CurrentParticleSelect->MinxRotationDir = MinDirection;

				float DirectionSize = CurrentParticleSelect->MaxRotationDir;
				ImGui::DragFloat ("Max Direction: ", &DirectionSize, 1, CurrentParticleSelect->MinxRotationDir, 99999);
				CurrentParticleSelect->MaxRotationDir = DirectionSize;

				float RotOffsetX = CurrentParticleSelect->RotationXOffset;
				ImGui::DragFloat ("Rotation Offset X: ", &RotOffsetX, 1, -360, 360);
				CurrentParticleSelect->RotationXOffset = RotOffsetX;

				float RotOffsetY = CurrentParticleSelect->RotationYOffset;
				ImGui::DragFloat ("Rotation Offset Y: ", &RotOffsetY, 1, -360, 360);
				CurrentParticleSelect->RotationYOffset = RotOffsetY;

				CurrentParticleSelect->Position.x = GetPosSD[0];
				CurrentParticleSelect->Position.y = GetPosSD[1];

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::End();
			}
		}


		if (CurrentAudioSourceSelect != nullptr) {
			if (ImGui::Begin("Audio Setting")) {
				char name[128];
				strcpy_s(name, CurrentAudioSourceSelect->Name.c_str());
				ImGui::InputText("Name Audio", name, ImGuiInputTextFlags_AutoSelectAll);
				CurrentAudioSourceSelect->Name = (string)name;

				float GetPosSD[2]{
					CurrentAudioSourceSelect->Position.x,
					CurrentAudioSourceSelect->Position.y,
				};

				ImGui::DragFloat2("Position: ", GetPosSD, 0.01f);

				CurrentAudioSourceSelect->Position.x = GetPosSD[0];
				CurrentAudioSourceSelect->Position.y = GetPosSD[1];

				ImGui::Spacing();
				ImGui::Spacing();

				char AudioName[128];
				strcpy_s(AudioName, CurrentAudioSourceSelect->AudioPath.c_str());
				ImGui::InputText("Audio Name", AudioName, ImGuiInputTextFlags_AutoSelectAll);
				CurrentAudioSourceSelect->AudioPath = (string)AudioName;

				if (ImGui::Button("Play Test")) {
					CurrentAudioSourceSelect->Play();
				}

				if (ImGui::Button("Stop Test")) {
					CurrentAudioSourceSelect->Stop();
				}
				ImGui::End();
			}
		}



		ImGui::PushID(12321);
		if (ViewEngineSettings) {
			if (ImGui::Begin("Editor Settings")) {
				int GetGridSize = wnd->GridSize;
				ImGui::DragInt("Grid Size: ", &GetGridSize);
				wnd->GridSize = GetGridSize;

				int* MoveSpeed = &Window::CAMERA_MAX_VEL;
				ImGui::InputInt("Camera Speed: ", MoveSpeed, 1);
				Window::CAMERA_MAX_VEL = *MoveSpeed;

				if (ImGui::Button("Close")) {
					ViewEngineSettings = false;
				}
				ImGui::End();
			}
		}
		ImGui::PopID();




		ImGui::PushID(6433);

		if (ViewWorldSettings) {
			if (ImGui::Begin("World Settings")) {

				char WorldName[256];

				strcpy_s(WorldName, ManagerScene::GetInstance()->GetCurrentScene()->SceneName.c_str());
				ImGui::InputText("Scene Name: ", WorldName, 100, ImGuiInputTextFlags_AutoSelectAll);
				ManagerScene::GetInstance()->GetCurrentScene()->SceneName = string(WorldName);
				ImGui::Spacing();


				b2Vec2 Gravity = ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->GetGravity();

				char TexturePath[100];
				strcpy_s(TexturePath, ManagerScene::GetInstance()->GetCurrentScene()->TexturePath.c_str());
				ImGui::InputText("Background: ", TexturePath, 100, ImGuiInputTextFlags_AutoSelectAll);

				ImGui::Image(TextureManager::LoadTexture(TexturePath), ImVec2(128, 128));
				ManagerScene::GetInstance()->GetCurrentScene()->TexturePath = (std::string)TexturePath;

				bool UseFullBG = ManagerScene::GetInstance()->GetCurrentScene()->UseFullScreen;
				ImGui::Checkbox("BG Fullscreen: ", &UseFullBG);
				ManagerScene::GetInstance()->GetCurrentScene()->UseFullScreen = UseFullBG;



				if (ImGui::Button("Save Sprites")) {
					ManagerScene::GetInstance()->GetCurrentScene()->TexturePath = TexturePath;
					ManagerScene::GetInstance()->GetCurrentScene()->SetNewBackground();
				}

				float GravityScaleGet[2]{
					ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->GetGravity().x,
					ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->GetGravity().y
				};

				ImGui::DragFloat2("Gravity Scale: ", GravityScaleGet);
				ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->SetGravity(b2Vec2(GravityScaleGet[0], GravityScaleGet[1]));


				bool Active = ManagerScene::GetInstance()->GetCurrentScene()->CompileScene;

				if (ImGui::Checkbox("Compile Scene?", &Active)) {
					string PathScene = "Assets/SaveData/" + ManagerScene::GetInstance()->GetCurrentScene()->SceneName + ".Scene";
					std::ifstream file(PathScene);
					std::cout << "Loading Path: " << PathScene;
					if (file) {
						json data;
						file >> data;
						bool NewValueCompiler = !(bool)data["CompileScene"];
						data["CompileScene"] = NewValueCompiler;
						ManagerScene::GetInstance()->GetCurrentScene()->CompileScene = (bool)data["CompileScene"];
						std::ofstream outfile(PathScene);
						outfile << data.dump(4);
					}
				}


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

			if (ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

				if (ImGui::BeginMenu("Create")) {
					if (ImGui::MenuItem("Game Object")) {
						ManagerScene::GetInstance()->GetCurrentScene()->SetupNewObject();
					}

					if (ImGui::MenuItem("Audio Source")) {
						ManagerScene::GetInstance()->GetCurrentScene()->SetupNewAudio();
					}

					if (ImGui::MenuItem("Particles System")) {
						ManagerScene::GetInstance()->GetCurrentScene()->SetupNewParticleSystem();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Scene Options")) {
					if (ImGui::MenuItem("New Scene")) {
						ManagerScene::GetInstance()->GetCurrentScene()->LoadScene("NewScene");
					}

					if (ImGui::MenuItem("Save Scene")) {
						ManagerScene::GetInstance()->GetCurrentScene()->SaveScene();
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
			if (ImGui::Begin("Script Editor", NULL, ImGuiWindowFlags_NoResize | ImGuiInputTextFlags_EnterReturnsTrue)) {

				if (SelectObject != nullptr) {

					ImGui::Text(SelectObject->name.c_str());
					if (ImGui::Button("Close")) {
						CodeEditor = false;
					}
					ImGui::Text("Press (CTRL + Enter) for jump line");

					ImVec2 Size;
					Size.x = 800;
					Size.y = 600;

					strcpy_s(str_hold, SelectObject->Script.c_str());

					ImGui::InputTextMultiline(" ", str_hold, 999999, Size, flags);
					SelectObject->Script = (string)str_hold;
					if (ImGui::Button("Compile")) {
						
						SelectObject->CompileLua();
					}
				}

				ImGui::End();
			}
		}



		if (InputSystem) {
			if (ImGui::Begin("Input Manager")) {

				if (ImGui::Button ("Close")) {
					InputSystem = false;
				}

				if (ImGui::Button ("Save")) {
					SaveData::GetInstance()->SaveEngineData();
				}

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				if (ImGui::Button("Create Axis")) {
					InputSystem::GetInstance()->SetNewInput("New Axis", "", "", "1", "-1");
				}

				ImGui::Spacing();
				ImGui::Spacing();

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

					char KeyNegateName[128];
					strcpy_s(KeyNegateName, InputSystem::GetInstance()->inputs[i].NegateKey.c_str());
					ImGui::InputText("Negate Key: ", KeyNegateName, ImGuiInputTextFlags_AutoSelectAll);
					InputSystem::GetInstance()->inputs[i].NegateKey = (string)KeyNegateName;

					char KeyAxis[128];
					strcpy_s(KeyAxis, InputSystem::GetInstance()->inputs[i].Axis.c_str());
					ImGui::InputText("Axis: VALUE +", KeyAxis, ImGuiInputTextFlags_AutoSelectAll);
					InputSystem::GetInstance()->inputs[i].Axis = (string)KeyAxis;

					char NegateKeyAxis[128];
					strcpy_s(NegateKeyAxis, InputSystem::GetInstance()->inputs[i].NegateAxis.c_str());
					ImGui::InputText("Negate Axis: VALUE -", NegateKeyAxis, ImGuiInputTextFlags_AutoSelectAll);
					InputSystem::GetInstance()->inputs[i].NegateAxis = (string)NegateKeyAxis;

					if (ImGui::Button ("Delete")) {
						auto it = InputSystem::GetInstance()->inputs.begin();
						std::advance(it, i);
						InputSystem::GetInstance()->inputs.erase(it);
					}

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

		if (ImGui::Begin("Console")) {
			if (ConsoleManager::GetInstance() != nullptr) {

				for (int i = 0; i < ConsoleManager::GetInstance()->Logs->size(); i++) {
					ImGui::PushID(i);
					string nmd = ConsoleManager::GetInstance()->GetLog(i);
					ImGui::Text(nmd.c_str());

					ImGui::PopID();
				}
			}
			ImGui::End();
		}

		/*

		ImGui::OpenPopup("Error Creating Image");
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Error Creating Image", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::SetItemDefaultFocus();
			ImGui::Text("The size of the Image must be greater than 0. Also it need to have a name!\n\n");

			ImGui::Separator();

			if (ImGui::Button("OK")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		*/


		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
};