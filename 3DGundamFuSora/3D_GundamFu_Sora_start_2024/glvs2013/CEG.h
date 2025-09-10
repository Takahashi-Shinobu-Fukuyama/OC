#pragma once

#include "CEnemyManager.h"
#include "CEnemy.h"
#include "Define.h"
#include <vector>

//Enemy Generator Class
class CEG {
public:
	static CEnemy m_Enemy;
	CEnemy myEnemy;
	CVector3D m_pos;
	int m_Gtime;

	static void Init();

	void Update();
	void SetPos(float x, float y, float z);
};