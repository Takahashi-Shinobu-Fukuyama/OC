#pragma once

#include "CEnemyManager.h"
#include "CEnemy.h"
#include "Define.h"
#include <vector>

class CEnemyGenerator2 {
public:
	static CEnemy m_Enemy;
	CVector3D m_pos;
	int m_Gtime;

	CEnemyGenerator2() : m_Gtime(ENEMY_GENERATE_TIME) {}
	CEnemyGenerator2(float x, float y, float z) : m_Gtime(ENEMY_GENERATE_TIME),m_pos(x,y,z) {}

	static void Init();

	void Update() {
		std::vector<CEnemy*> vec = CEnemyManager::GetInstance()->GetVector();
		m_Gtime--;
		if (m_Gtime < 0 && vec.size() < MAX_ENEMY) {
			CEnemy *e = new CEnemy();
			*e = m_Enemy;
			e->setPos(m_pos);
			e->m_TargetPos = e->getPos();
			CEnemyManager::GetInstance()->Add(e);
			m_Gtime = ENEMY_GENERATE_TIME;
		}		
	}
};