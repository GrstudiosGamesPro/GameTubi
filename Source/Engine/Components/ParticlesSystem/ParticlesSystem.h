#pragma once
#include "../Gizmos/GizmoDrawer.h"
#include "../Mathf/Vector2.h"
#include "../Object.h"
#include "../Mathf/GMathf.h"
#include "Particle.h"
 
using namespace std;

class ParticlesSystem
{

public:
	GizmoDrawer* drawer = new GizmoDrawer();
	//Variables Guardables
	string Name;
	int LifeTime = 2;
	string ParticlePath = "Assets/Sprites/ParticleExample.png";
	float MaxRotationDir = 0;
	float MinxRotationDir = 0;
	float RotationXOffset = 0;
	float RotationYOffset = 360;
	float Speed = 0.01;
	float SpawnTime = 10;
	Vector2 Position;
	int MaxParticles = 100;
	bool Stop = false;

	//Variables No Guardables
	float DefaultTimeSpawn;
	int Diametro = LifeTime * 2;


	vector<Particle*> Particles = vector<Particle*>();

	void Start() {
		if (drawer != nullptr) {
			drawer->Start("Particle");
		}
	}

	void SetPosition(float PosX, float PosY) {
		Position = Vector2(PosX, PosY);
	}

	void Draw() {
		if (!Stop) {

			if (drawer != nullptr) {
				drawer->Draw(Position.x, Position.y);
			}

			SDL_SetRenderDrawColor(Window::renderer, 255, 0, 0, 255);

			int GizmosPosX = Position.x * 32;
			int GizmosPosY = Position.y * 32;

			int segments = 100;
			float angleStep = 360.0f / (float)segments;
			float currentAngle = 0.0f;

			int x1, y1, x2, y2;

			for (int i = Particles.size(); i < MaxParticles; i++) {
				if (DefaultTimeSpawn <= 0) {
					SpawnParticle();
					DefaultTimeSpawn = SpawnTime;
				}
				else {
					DefaultTimeSpawn -= 1;
				}
			}

			for (int i = 0; i < Particles.size(); i++)
			{
				Particles[i]->Speed = Speed;
				Particles[i]->RotationXOffset = RotationXOffset;
				Particles[i]->RotationYOffset = RotationYOffset;
				Particles[i]->Draw();

				if (Particles[i]->DefaultTime <= 0) {
					auto it = Particles.begin();
					std::advance(it, i);
					delete* (&Particles[i]);
					Particles.erase(it);
				}
				else {
					Particles[i]->DefaultTime -= 1;
				}
			}
		}
	}

	void SpawnParticle() {
		Particle* ParticleText = new Particle();
		ParticleText->TexturePath = ParticlePath;
		SDL_Surface* image = IMG_Load(ParticleText->TexturePath.c_str());
		ParticleText->texture = SDL_CreateTextureFromSurface(Window::renderer, image);
		SDL_FreeSurface(image);

		float centerX = Position.x;
		float centerY = Position.y;

		ParticleText->PosX = centerX;
		ParticleText->PosY = centerY;

		ParticleText->DirectionX = GMathf::RandomFloat	(MinxRotationDir, MaxRotationDir);
		ParticleText->DirectionY = GMathf::RandomFloat	(MinxRotationDir, MaxRotationDir);
		ParticleText->DefaultTime = LifeTime * 100;
		Particles.push_back(ParticleText);
	}

	void ClearParticles() {
		for (Particle* particle : Particles) {
			delete particle;
		}

		for (int i = Particles.size() - 1; i >= 0; i--) {
			Particles.erase(Particles.begin() + i);
		}

		Particles.clear();
	}

	~ParticlesSystem() {
		for (Particle* particle : Particles) {
			SDL_DestroyTexture (particle->texture);
			delete particle;
		}

		for (int i = Particles.size() - 1; i >= 0; i--) {
			Particles.erase(Particles.begin() + i);
			std::cout << "Total objetos" << endl;
		}

		Particles.clear();
	}
};