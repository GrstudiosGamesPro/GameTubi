#include "PlayMode.h"


PlayMode* PlayMode::m_engine = nullptr;

PlayMode::PlayMode() {

}

void PlayMode::create()
{
	if (PlayMode::m_engine) throw std::exception("Play Mode already created.");
	PlayMode::m_engine = new PlayMode();
	std::cout << "Play Mode Created" << endl;
	GetInstance()->IsPlaying = false;
}


PlayMode* PlayMode::GetInstance()
{
	return m_engine;
}

void PlayMode::release()
{
	if (!PlayMode::m_engine) return;
	delete PlayMode::m_engine;
}

PlayMode::~PlayMode()
{
	PlayMode::m_engine = nullptr;
}


void PlayMode::Start () {
	IsPlaying = true;

	SaveData::GetInstance()->Save();
	SaveData::GetInstance()->SaveEngineData();

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {
		Object* getOBJ = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];

		if (getOBJ->RunLua) {
			getOBJ->CompileLua();
		}
	}
}

void PlayMode::Stop  (){
	IsPlaying = false;
	SaveData::GetInstance()->Load ("Assets/SaveData/SampleScene.scene");
}