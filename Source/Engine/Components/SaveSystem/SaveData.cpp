#include "SaveData.h"
#include "../SceneManager/ManagerScene.h"
#include "../InputSystem/InputSystem.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;


SaveData* SaveData::m_engine = nullptr;

SaveData::SaveData() {

}

void SaveData::create()
{
    if (SaveData::m_engine) throw std::exception("SaveManager already created.");
    SaveData::m_engine = new SaveData();
    std::cout << "SaveManager Created" << endl;
}


SaveData* SaveData::GetInstance()
{
    return m_engine;
}

void SaveData::release()
{
    if (!SaveData::m_engine) return;
    delete SaveData::m_engine;
}

SaveData::~SaveData()
{
    SaveData::m_engine = nullptr;
}

void SaveData::Save() {
    if (ManagerScene::GetInstance() != nullptr) {
        json ObjectData;

        ofstream outdata;
        json Objects = json::array();
        json ObjectsAudio = json::array();
        json AxisKey = json::array();
        json ObjectsChildren = json::array();

        for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
            ObjectsChildren.clear();
            json ObjectsValues;
            Object& obj = *ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];

                ObjectsValues["ObjectName"] = obj.GetName();
                ObjectsValues["PosX"] = obj.GetPosition().x;
                ObjectsValues["PosY"] = obj.GetPosition().y;
                ObjectsValues["IsStatic"] = obj.IsStatic;
                ObjectsValues["IsTrigger"] = obj.IsTrigger;


                ObjectsValues["ScaleX"] = obj.ScaleX;
                ObjectsValues["ScaleY"] = obj.ScaleY;

                ObjectsValues["ScaleBoxX"] = obj.ScaleBoxX;
                ObjectsValues["ScaleBoxY"] = obj.ScaleBoxY;

                ObjectsValues["SpritePath"] = obj.TexturePath;
                ObjectsValues["ColisionActive"] = obj.useGravity;

                ObjectsValues["Density"] = obj.density;
                ObjectsValues["Friction"] = obj.friction;

                ObjectsValues["AngleControlBody"] = obj.ControlAngleBody;

                ObjectsValues["Scripting"] = obj.Script;
                ObjectsValues["RunLua"] = obj.RunLua;
                ObjectsValues["Tag"] = obj.Tag;

                if (obj.Childrens.size() > 0) {
                    for (int e = 0; e < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->Childrens.size(); e++) {
                        json ChildrenData;
                        Object& objChild = *ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->Childrens[e];

                        ChildrenData["ObjectName"] = objChild.GetName();
                        ChildrenData["IsStatic"] = objChild.IsStatic;
                        ChildrenData["IsTrigger"] = objChild.IsTrigger;

                        ChildrenData["PosX"] = objChild.GetPosition().x;
                        ChildrenData["PosY"] = objChild.GetPosition().y;

                        ChildrenData["ScaleX"] = objChild.ScaleX;
                        ChildrenData["ScaleY"] = objChild.ScaleY;

                        ChildrenData["ScaleBoxX"] = objChild.ScaleBoxX;
                        ChildrenData["ScaleBoxY"] = objChild.ScaleBoxY;

                        ChildrenData["SpritePath"] = objChild.TexturePath;
                        ChildrenData["ColisionActive"] = objChild.useGravity;

                        ChildrenData["Density"] = objChild.density;
                        ChildrenData["Friction"] = objChild.friction;

                        ChildrenData["AngleControlBody"] = objChild.ControlAngleBody;
                        
                        ChildrenData["Scripting"] = objChild.Script;
                        ChildrenData["RunLua"] = objChild.RunLua;
                        ChildrenData["Tag"] = objChild.Tag;


                        ObjectsChildren.push_back(ChildrenData);
                    }
                }


                if (ObjectsChildren.size() > 0) {
                    ObjectsValues["ChildObjects"] = ObjectsChildren;
                }

                Objects.push_back(ObjectsValues);
            
        }

        for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Audio.size(); i++) {
            json AudioValue;
            AudioSource* source = ManagerScene::GetInstance()->GetCurrentScene()->Audio[i];

            AudioValue["Name"] = source->Name;
            AudioValue["AudioPath"] = source->AudioPath;
            AudioValue["PosX"] = source->Position.x;
            AudioValue["PosY"] = source->Position.y;
            ObjectsAudio.push_back (AudioValue);
        }

        json ParticlesToSave = json::array();


        for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Particles.size(); i++) {
            json ParticlesSystems;
            ParticlesSystem* source = ManagerScene::GetInstance()->GetCurrentScene()->Particles[i];

            ParticlesSystems["Name"] = source->Name;
            ParticlesSystems["PosX"] = source->Position.x;
            ParticlesSystems["PosY"] = source->Position.y;
            ParticlesSystems["ParticleTexturePath"] = source->ParticlePath;
            ParticlesSystems["ParticleTimeSpawn"] = source->SpawnTime;
            ParticlesSystems["LifeTime"] = source->LifeTime;
            ParticlesSystems["Speed"] = source->Speed;
            ParticlesSystems["OffsetX"] = source->RotationXOffset;
            ParticlesSystems["OffsetY"] = source->RotationYOffset;
            ParticlesSystems["MaxRotation"] = source->MaxRotationDir;
            ParticlesSystems["MinRotation"] = source->MinxRotationDir;
            ParticlesSystems["MaxParticles"] = source->MaxParticles;
            ParticlesToSave.push_back(ParticlesSystems);
        }


        ObjectData["AudioSources"] = ObjectsAudio;
        ObjectData["Particles"] = ParticlesToSave;
        ObjectData["Objetos"] = Objects;

        string SceneGettedName = ManagerScene::GetInstance()->GetCurrentScene()->SceneName;
        ObjectData["SceneName"] = SceneGettedName;

        string BackgroundPath = ManagerScene::GetInstance()->GetCurrentScene()->TexturePath;
        ObjectData["BackgroundPath"] = BackgroundPath;

        bool FullScreen = ManagerScene::GetInstance()->GetCurrentScene()->UseFullScreen;
        ObjectData["BGFullScreen"] = FullScreen;
        
        
        bool CompileScene = ManagerScene::GetInstance()->GetCurrentScene()->CompileScene;
        ObjectData["CompileScene"] = CompileScene;


        outdata.open("Assets/SaveData/" + ManagerScene::GetInstance()->GetCurrentScene()->SceneName + ".Scene");
        outdata << ObjectData.dump (4);



        if (!outdata) {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        std::cout << "Data saved" << endl;
    }


    SaveEngineData();
}


void SaveData::SaveEngineData() {
    if (InputSystem::GetInstance() != nullptr) {
        json ObjectData;

        ofstream outdata;
        json AxisKey = json::array();

        for (int i = 0; i < InputSystem::GetInstance()->inputs.size(); i++) {
            json ObjectsValues;

            ObjectsValues["AxisName"] = InputSystem::GetInstance()->inputs[i].Name;
            ObjectsValues["Key"] = InputSystem::GetInstance()->inputs[i].Key;
            ObjectsValues["NegateKey"] = InputSystem::GetInstance()->inputs[i].NegateKey;
            ObjectsValues["Axis"] = InputSystem::GetInstance()->inputs[i].Axis;
            ObjectsValues["NegateAxis"] = InputSystem::GetInstance()->inputs[i].NegateAxis;

            AxisKey.push_back(ObjectsValues);
        }
        ObjectData["Inputs"] = AxisKey;

        outdata.open("Assets/ProyectSettings/ProyectSettings.gmt");
        outdata << ObjectData.dump(4);

        if (!outdata) {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
    }
}

void SaveData::LoadEngineData() {
    if (InputSystem::GetInstance() != nullptr) {
        std::ifstream file("Assets/ProyectSettings/ProyectSettings.gmt");

        if (file) {
            json data;
            file >> data;
            nlohmann::json Inputs = data["Inputs"];

            for (int i = 0; i < Inputs.size(); i++) {
                InputSystem::Inputs* NewInput = new InputSystem::Inputs();

                NewInput->Name = Inputs[i]["AxisName"];
                NewInput->NegateKey = Inputs[i]["NegateKey"];
                NewInput->Axis = Inputs[i]["Axis"];
                NewInput->NegateAxis = Inputs[i]["NegateAxis"];
                NewInput->Key = Inputs[i]["Key"];
                InputSystem::GetInstance()->inputs.push_back (*NewInput);
            }

            file.close();
        }
    }
}


void SaveData::Load (string PathScene) {
    if (ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld == nullptr) {
        ManagerScene::GetInstance()->GetCurrentScene()->CreateGravity();
        std::cout << "Creando gravedad" << endl;
    }

    if (ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld != nullptr) {
        for (auto objT : ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene) {
            ManagerScene::GetInstance()->GetCurrentScene()->DestroyObject (objT);
        }

        for (ParticlesSystem* d : ManagerScene::GetInstance()->GetCurrentScene()->Particles) {
            d->Stop = true;
            d->ClearParticles();
        }

        if (ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->GetBodyCount() > 0) {
            ManagerScene::GetInstance()->GetCurrentScene()->UnLoadAllBodys();
        }

        /*
        for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->Particles.size(); i++)
        {
            ManagerScene::GetInstance()->GetCurrentScene()->Particles[i]->Stop = true;
            ManagerScene::GetInstance()->GetCurrentScene()->Particles[i]->Clear();
            auto it = ManagerScene::GetInstance()->GetCurrentScene()->Particles.begin();
            std::advance(it, i);
            delete* (&ManagerScene::GetInstance()->GetCurrentScene()->Particles[i]);
            ManagerScene::GetInstance()->GetCurrentScene()->Particles.erase(it);
        }
        */
       // ManagerScene::GetInstance()->GetCurrentScene()->Particles.clear();
        ManagerScene::GetInstance()->GetCurrentScene()->Audio.clear();

        delete ManagerScene::GetInstance()->GetCurrentScene()->backgroundTexture;
        SDL_DestroyTexture (ManagerScene::GetInstance()->GetCurrentScene()->backgroundTexture);
    }


    std::ifstream file (PathScene);

    if (file) {
        json data;
        file >> data;

        std::string nombre = (string)data["SceneName"];
        ManagerScene::GetInstance()->GetCurrentScene()->TexturePath = (string)data["BackgroundPath"];
        ManagerScene::GetInstance()->GetCurrentScene()->UseFullScreen = (bool)data["BGFullScreen"];
        ManagerScene::GetInstance()->GetCurrentScene()->CompileScene = (bool)data["CompileScene"];
        ManagerScene::GetInstance()->GetCurrentScene()->SetNewBackground();
        nlohmann::json ObjectsArray = data["Objetos"];
        nlohmann::json ObjectsAudioArray = data["AudioSources"];
        nlohmann::json ParticlesArray = data["Particles"];


        for (int i = 0; i < ObjectsArray.size(); i++) {
            Object* OBJ = new Object();
            OBJ->pos.x = (float)ObjectsArray[i]["PosX"];
            OBJ->pos.y = (float)ObjectsArray[i]["PosY"];
            OBJ->Start();
            OBJ->Script = (string)ObjectsArray[i]["Scripting"];

            OBJ->SetName(ObjectsArray[i]["ObjectName"]);


            OBJ->ScaleX = (float)ObjectsArray[i]["ScaleX"];
            OBJ->ScaleY = (float)ObjectsArray[i]["ScaleY"];

            OBJ->ScaleBoxX = (float)ObjectsArray[i]["ScaleBoxX"];
            OBJ->ScaleBoxY = (float)ObjectsArray[i]["ScaleBoxY"];

            OBJ->useGravity = (bool)ObjectsArray[i]["ColisionActive"];
            OBJ->density = (float)ObjectsArray[i]["Density"];
            OBJ->friction = (float)ObjectsArray[i]["Friction"];

            OBJ->IsStatic = (bool)ObjectsArray[i]["IsStatic"];
            OBJ->IsTrigger = (bool)ObjectsArray[i]["IsTrigger"];

            OBJ->TexturePath = (string)ObjectsArray[i]["SpritePath"];
            OBJ->ControlAngleBody = (bool)ObjectsArray[i]["AngleControlBody"];
            OBJ->RunLua = (bool)ObjectsArray[i]["RunLua"];
            OBJ->Tag = (string)ObjectsArray[i]["Tag"];
            OBJ->SetNewTexture();

            nlohmann::json ObjectsChildren = ObjectsArray[i]["ChildObjects"];



            if (ObjectsChildren.size() > 0) {
                for (int e = 0; e < ObjectsChildren.size(); e++) {
                    Object* OBJa = new Object();
                    OBJa->IsStatic = (bool)ObjectsChildren[e]["IsStatic"];
                    OBJa->IsTrigger = (bool)ObjectsChildren[e]["IsTrigger"];
                    OBJa->pos.x = (float)ObjectsChildren[e]["PosX"];
                    OBJa->pos.y = (float)ObjectsChildren[e]["PosY"];
                    OBJa->Start();
                    OBJa->Parent = OBJ;
                    OBJa->Script = (string)ObjectsChildren[e]["Scripting"];

                    OBJa->SetName((string)ObjectsChildren[e]["ObjectName"]);


                    OBJa->ScaleX = (float)ObjectsChildren[e]["ScaleX"];
                    OBJa->ScaleY = (float)ObjectsChildren[e]["ScaleY"];

                    OBJa->ScaleBoxX = (float)ObjectsChildren[e]["ScaleBoxX"];
                    OBJa->ScaleBoxY = (float)ObjectsChildren[e]["ScaleBoxY"];

                    OBJa->useGravity = (bool)ObjectsChildren[e]["ColisionActive"];
                    OBJa->density = (float)ObjectsChildren[e]["Density"];
                    OBJa->friction = (float)ObjectsChildren[e]["Friction"];


                    OBJa->TexturePath = (string)ObjectsChildren[e]["SpritePath"];
                    OBJa->ControlAngleBody = (bool)ObjectsChildren[e]["AngleControlBody"];
                    OBJa->SetNewTexture();
                    OBJa->Tag = (string)ObjectsChildren[e]["Tag"];

                    OBJa->RunLua = (bool)ObjectsChildren[i]["RunLua"];

                    OBJ->Childrens.push_back (OBJa);
                    ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.push_back(OBJa);
                }
            }

            OBJ->SetNewTexture();
            ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.push_back(OBJ);
        }

        for (int i = 0; i < ObjectsAudioArray.size(); i++) {
            AudioSource* newSource = new AudioSource();
            newSource->Name = (string)ObjectsAudioArray[i]["Name"];
            newSource->AudioPath = (string)ObjectsAudioArray[i]["AudioPath"];
            newSource->Position.x = (float)ObjectsAudioArray[i]["PosX"];
            newSource->Position.y = (float)ObjectsAudioArray[i]["PosY"];
            newSource->Start();
            ManagerScene::GetInstance()->GetCurrentScene()->Audio.push_back (newSource);
        }

        std::cout << "Particulas totales: " << ManagerScene::GetInstance()->GetCurrentScene()->Particles.size() << endl;

        for (int i = 0; i < ParticlesArray.size(); i++) {
            ParticlesSystem* newSource = new ParticlesSystem();
            newSource->Name = (string)ParticlesArray[i]["Name"];
            newSource->Position.x = (float)ParticlesArray[i]["PosX"];
            newSource->Position.y = (float)ParticlesArray[i]["PosY"];
            newSource->ParticlePath = (string)ParticlesArray[i]["ParticleTexturePath"];
            newSource->SpawnTime = (float)ParticlesArray[i]["ParticleTimeSpawn"];
            newSource->LifeTime = (int)ParticlesArray[i]["LifeTime"];
            newSource->Speed = (float)ParticlesArray[i]["Speed"];
            newSource->RotationXOffset = (float)ParticlesArray[i]["OffsetX"];
            newSource->RotationYOffset = (float)ParticlesArray[i]["OffsetY"];
            newSource->MaxRotationDir = (float)ParticlesArray[i]["MaxRotation"];
            newSource->MinxRotationDir = (float)ParticlesArray[i]["MinRotation"];
            newSource->MaxParticles = (int)ParticlesArray[i]["MaxParticles"];
            newSource->Start();

            ManagerScene::GetInstance()->GetCurrentScene()->Particles.push_back (newSource);
            //ManagerScene::GetInstance()->GetCurrentScene()->SetupNewParticleSystem (newSource);
        }


        file.close();
    }
}