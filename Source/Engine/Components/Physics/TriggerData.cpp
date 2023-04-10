#include "TriggerData.h"
#include "../SceneManager/ManagerScene.h"

void TriggerData::BeginContact (b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {

		Object* GetOBJ = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];

		if (GetOBJ->body == bodyA) {
			if (GetOBJ->IsTrigger) {
				for (int e = 0; e < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); e++) {
					Object* GetOBJB = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[e];

					if (GetOBJB->body == bodyB) {
						GetOBJ->OnTriggerStart (GetOBJB);
					}
				}
			}
		}


		if (GetOBJ->body == bodyB) {
			if (GetOBJ->IsTrigger) {
				for(int e = 0; e < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); e++) {
					Object* GetOBJA = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[e];

					if (GetOBJA->body == bodyA) {
						GetOBJ->OnTriggerStart(GetOBJA);
					}
				}
			}
		}
	}
}

void TriggerData::EndContact	(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	for (int i = 0; i < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); i++) {

		Object* GetOBJ = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[i];

		if (GetOBJ->body == bodyA) {
			if (GetOBJ->IsTrigger) {
				for (int e = 0; e < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); e++) {
					Object* GetOBJB = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[e];

					if (GetOBJB->body == bodyB) {
						GetOBJ->OnTriggerStart(GetOBJB);
					}
				}
			}
		}


		if (GetOBJ->body == bodyB) {
			if (GetOBJ->IsTrigger) {
				for (int e = 0; e < ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene.size(); e++) {
					Object* GetOBJA = ManagerScene::GetInstance()->GetCurrentScene()->ObjectsInScene[e];

					if (GetOBJA->body == bodyA) {
						GetOBJ->OnTriggerStart(GetOBJA);
					}
				}
			}
		}
	}
}