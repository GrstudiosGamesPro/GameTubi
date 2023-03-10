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
#include "../../WindowsCompiler/WindowsCompiler.h"
#include "../AudioSource/AudioSource.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace std;

class ImGuiImplement {
public:
	Window* wnd;
	SDL_Texture* texture;
	Object* SelectObject;
	AudioSource* CurrentAudioSourceSelect;

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

	

		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem ("New Scene")) {
					ManagerScene::GetInstance()->GetCurrentScene()->LoadScene ("NewScene");
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

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}


		if (AnimatorWindows) {
			if (ImGui::Begin ("Animator")) {


				ImGui::End();
			}
		}


		ImGui::PushID (252334);

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
				ImGui::End();
			}
		}

		ImGui::PopID();

		if (ImGui::Begin("Hierarchy")) {

			if (ImGui::TreeNode ("Workspace")) {
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
				ImGui::TreePop();
			}


			if (ImGui::TreeNode ("Audio")) {

				for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Audio.size(); i++) {
					ImGui::PushID(i);
					if (ImGui::Button (ManagerScene::GetInstance()->GetCurrentScene()->Audio[i]->Name.c_str())) {
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
			ImGui::End();
		}


		if (CurrentAudioSourceSelect != nullptr) {
			if (ImGui::Begin ("Audio Setting")) {
				char name[128];
				strcpy_s(name, CurrentAudioSourceSelect->Name.c_str());
				ImGui::InputText ("Name Audio", name, ImGuiInputTextFlags_AutoSelectAll);
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
				ImGui::InputText ("Audio Name", AudioName, ImGuiInputTextFlags_AutoSelectAll);
				CurrentAudioSourceSelect->AudioPath = (string)AudioName;

				if (ImGui::Button ("Play Test")) {
					CurrentAudioSourceSelect->Play();
				}

				if (ImGui::Button ("Stop Test")) {
					CurrentAudioSourceSelect->Stop();
				}
				ImGui::End();
			}
		}


		if (SelectObject != nullptr) {
			if (ImGui::Begin("Inspector")) {
				if (ImGui::Button("Close")) 
				{
					SelectObject = nullptr;
				}
				else {
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

		ImGui::PushID (12321);
		if (ViewEngineSettings) {
			if (ImGui::Begin("Editor Settings")) {
				int GetGridSize = wnd->GridSize;
				ImGui::DragInt("Grid Size: ", &GetGridSize);
				wnd->GridSize = GetGridSize;

				int* MoveSpeed = &Window::CAMERA_MAX_VEL;
				ImGui::InputInt ("Camera Speed: ", MoveSpeed, 1);
				Window::CAMERA_MAX_VEL = *MoveSpeed;

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

			if (ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

				if (ImGui::BeginMenu("Create")) {
					if (ImGui::MenuItem("Game Object")) {
						ManagerScene::GetInstance()->GetCurrentScene()->SetupNewObject();
					}

					if (ImGui::MenuItem("Audio Source")) {
						ManagerScene::GetInstance()->GetCurrentScene()->SetupNewAudio();
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
				ImGui::Text (SelectObject->name.c_str());
				if (ImGui::Button("Close")) {
					CodeEditor = false;
				}
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