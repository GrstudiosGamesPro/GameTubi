#pragma once
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class SaveData
{

private:
    static SaveData* m_engine;


public:
    SaveData();
    ~SaveData();

    static void create();
    static void release();
    static SaveData* GetInstance();

    void Save();
    void Load();
};