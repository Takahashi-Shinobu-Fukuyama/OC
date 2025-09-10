#pragma once
#include "CMatrix.h"
#include "CEnemy.h"
#include "CEnemyManager.h"

struct SGenerateTable {
	int time;
	int func;
	CVector3D pos;
};


class CEnemyGenerator {
public:
	int m_time;
	int m_i;
	SGenerateTable *m_pdata;
	CEnemy m_Enemy;

	CEnemyGenerator() : m_i(0), m_time(0) {
		static SGenerateTable data[] = 
		{
			100, 1, { 10.0f, 0.0f, 50.0f },
			200, 1, { 10.0f, 0.0f, 50.0f },
			300, 1, { 10.0f, 0.0f, 50.0f },
			400, 9, { 10.0f, 0.0f, 30.0f }
		};

		m_pdata = data;	
	};

	void Init() {
		m_Enemy.Init();
	}

	~CEnemyGenerator() {
		/*
		if (!m_pdata) {
			delete[] m_pdata;
			m_pdata = 0;
		}
		*/
	}

	void Update() {
		if (m_time < 0)
			return;

		m_time++;

		if (m_time == m_pdata[m_i].time) {
			switch (m_pdata[m_i].func) {
			case 9:
				m_time = -1;
				break;
			case 1:
				CEnemy *e = new CEnemy();
				*e = m_Enemy;
				e->setPos(m_pdata[m_i].pos);
				e->m_TargetPos = e->getPos();
				CEnemyManager::GetInstance()->Add(e);
				m_i++;
				break;
			}
		}

	}
};