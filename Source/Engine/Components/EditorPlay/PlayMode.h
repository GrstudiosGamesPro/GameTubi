#pragma once
#include "../SceneManager/Scene.h"
#include "../SceneManager/ManagerScene.h"
#include "../SaveSystem/SaveData.h"
#include "../Object.h"

class PlayMode
{    
    static PlayMode* m_engine;
public:
    PlayMode();
    ~PlayMode();

    static void create();
    static void release();
    static PlayMode* GetInstance();

	bool IsPlaying;

	void Start ();
	void Stop  ();
};