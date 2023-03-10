#include "SaveData.h"
#include "../SceneManager/ManagerScene.h"
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

        for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
            json ObjectsValues;
            Object& obj = *ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];

            ObjectsValues["ObjectName"] = obj.GetName();
            ObjectsValues["PosX"] = obj.pos.x;
            ObjectsValues["PosY"] = obj.pos.y;

            ObjectsValues["ScaleX"] = obj.ScaleX;
            ObjectsValues["ScaleY"] = obj.ScaleY;

            ObjectsValues["ScaleBoxX"] = obj.ScaleBoxX;
            ObjectsValues["ScaleBoxY"] = obj.ScaleBoxY;

            ObjectsValues["SpritePath"] = obj.TexturePath;
            ObjectsValues["ColisionActive"] = obj.useGravity;

            ObjectsValues["Density"] = obj.density;
            ObjectsValues["Friction"] = obj.friction;

            ObjectsValues["Scripting"] = obj.Script;

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

        ObjectData["AudioSources"] = ObjectsAudio;
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
    }
}


void SaveData::Load (string PathScene) {
    if (ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld != nullptr && ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->GetBodyCount() > 0) {
        ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld = nullptr;
        //ManagerScene::GetInstance()->GetCurrentScene()->UnLoadAllBodys();

        for (int i = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size() - 1; i >= 0; i--) {
            //ManagerScene::GetInstance()->GetCurrentScene()->GravityWorld->DestroyBody (ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i]->body);
            ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.erase(ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.begin() + i);
        }
        ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.clear();
        ManagerScene::GetInstance()->GetCurrentScene()->CreateGravity();

        for (int i = ManagerScene::GetInstance()->GetCurrentScene()->Audio.size() - 1; i >= 0; i--) {
            ManagerScene::GetInstance()->GetCurrentScene()->Audio[i]->Stop();
            ManagerScene::GetInstance()->GetCurrentScene()->Audio.erase (ManagerScene::GetInstance()->GetCurrentScene()->Audio.begin() + i);
        }
    }

    std::ifstream file (PathScene);
    std::cout << "Loading Path: " << PathScene;
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


        for (int i = 0; i < ObjectsArray.size(); i++) {

            Object* OBJ = new Object();
            OBJ->pos.x = (float)ObjectsArray[i]["PosX"];
            OBJ->pos.y = (float)ObjectsArray[i]["PosY"];
            OBJ->Script = (string)ObjectsArray[i]["Scripting"];

            OBJ->SetName(ObjectsArray[i]["ObjectName"]);


            OBJ->ScaleX = (float)ObjectsArray[i]["ScaleX"];
            OBJ->ScaleY = (float)ObjectsArray[i]["ScaleY"];

            OBJ->ScaleBoxX = (float)ObjectsArray[i]["ScaleBoxX"];
            OBJ->ScaleBoxY = (float)ObjectsArray[i]["ScaleBoxY"];

            OBJ->TexturePath = ObjectsArray[i]["SpritePath"];
            OBJ->SetNewTexture();


            OBJ->useGravity = (bool)ObjectsArray[i]["ColisionActive"];
            OBJ->density = (float)ObjectsArray[i]["Density"];
            OBJ->friction = (float)ObjectsArray[i]["Friction"];
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

        file.close();
    }
}