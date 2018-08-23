#pragma once
#include "main.h"
#include "cscene.h"

class Bullet : public CScene{
public:
	//Bullet(int priority) : CScene(priority, SCENE_TYPE_BULLET) {}
	~Bullet() {}
public:
	void Init() override;
	void Update() override {
		for (int i =  0; i < 10; i++)
		{
			if (m_Scene[i]->GetType() == SCENE_TYPE_ENEMY)
			{
				//CEnemy* enemy = (CEnemy*)m_Scene[i];
				//enemyPosition = enemy->GetPosition();
				//if (/*è’ìÀîªíË*/)
				{

				}
			}
		}
	}
	void Draw() override;
	void Uninit() override;
private:
	int priority_;
	CScene* m_Scene[10];
};