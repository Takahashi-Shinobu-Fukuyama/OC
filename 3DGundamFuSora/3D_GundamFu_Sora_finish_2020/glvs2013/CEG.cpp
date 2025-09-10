#include "CEG.h"
#include "CScene.h"

CEnemy CEG::m_Enemy;
void CEG::Init() {
	m_Enemy.Init();
}

void  CEG::Update() {
	if (myEnemy.m_Stop) {
		myEnemy.setPos(m_pos.x + CScene::m_player.m_pos.x, m_pos.y + CScene::m_player.m_pos.y, m_pos.z + CScene::m_player.m_pos.z);
		myEnemy.m_Stop = false;
	}
}

void  CEG::SetPos(float x, float y, float z) {
	myEnemy = m_Enemy;
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
	myEnemy.setPos(m_pos);
	myEnemy.m_Stop = false;
	CEnemyManager::GetInstance()->Add(&myEnemy);
}

