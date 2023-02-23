#pragma once
#include <direct.h>
#include <iostream>
#include <filesystem>

using namespace std;

class WindowsCompiler
{
public:
	void CompileGame() {
        std::string GamePathBuild = "GameBuild";
        int resultado = _mkdir(GamePathBuild.c_str());

        if (resultado == 0) {
            std::cout << "Carpeta creada exitosamente" << std::endl;
        }
        else {
            std::cout << "Error al crear la carpeta" << std::endl;
        }


        std::string GameContentBuild = "GameBuild/Content";
        int _resultado = _mkdir(GameContentBuild.c_str());

        if (_resultado == 0) {
            std::cout << "Carpeta creada exitosamente" << std::endl;
        }
        else {
            std::cout << "Error al crear la carpeta" << std::endl;
        }

        std::string AssetsPathCreate = "GameBuild/Content/GameAssets/";
        int _AssetsPathResult = _mkdir(AssetsPathCreate.c_str());

        if (_AssetsPathResult == 0) {
            std::cout << "Carpeta creada exitosamente" << std::endl;
        }
        else {
            std::cout << "Error al crear la carpeta" << std::endl;
        }

        std::string AssetsPathCreateScene = "GameBuild/Content/GameAssets/Scene";
        int _AssetsPathResultScene = _mkdir(AssetsPathCreateScene.c_str());

        if (_AssetsPathResultScene == 0) {
            std::cout << "Carpeta creada exitosamente" << std::endl;
        }
        else {
            std::cout << "Error al crear la carpeta" << std::endl;
        }

        std::string AssetsPathCreateGameFiles = "GameBuild/Content/GameAssets/GameFiles";
        int _AssetsPathResultGameFiles = _mkdir(AssetsPathCreateGameFiles.c_str());

        if (_AssetsPathResultGameFiles == 0) {
            std::cout << "Carpeta creada exitosamente" << std::endl;
        }
        else {
            std::cout << "Error al crear la carpeta" << std::endl;
        }



        std::string carpeta_origen = "Assets/Sprites/";
        std::string carpeta_destino = "GameBuild/Content/GameAssets/GameFiles/";

        for (const auto& archivo : std::filesystem::directory_iterator(carpeta_origen)) {
            std::string ruta_archivo_destino = carpeta_destino + "/" + archivo.path().filename().string();
            std::filesystem::copy(archivo, ruta_archivo_destino, std::filesystem::copy_options::overwrite_existing);
        }

        std::string CarpetaDeEscenas = "Assets/SaveData/";
        std::string CarpetaDeEscenasDestino = "GameBuild/Content/GameAssets/Scene/";

        for (const auto& archivo : std::filesystem::directory_iterator(CarpetaDeEscenas)) {
            std::string ruta_archivo_destino = CarpetaDeEscenasDestino + "/" + archivo.path().filename().string();
            std::filesystem::copy(archivo, ruta_archivo_destino, std::filesystem::copy_options::overwrite_existing);
        }
	}
};