#include "CBeamHoming.h"
#include "CEnemyManager.h"

void CBeamHoming::Start(const CVector3D& pos, const CVector3D& target) {
	CBeam::Start(pos, target);
	m_start_target_pos = target;
	CVector3D dir_n = target - pos;
	dir_n.Normalize();
	float max_dot = 0.0f;
	CVector3D enemyBeamDir;
	std::list<CEnemy*> array = CEnemyManager::GetInstance()->GetArray();
	std::list<CEnemy*>::iterator itr;
	for (itr = array.begin(); itr != array.end(); itr++) {
		enemyBeamDir = (*itr)->m_pos - pos;
		enemyBeamDir.Normalize();
		if (max_dot < enemyBeamDir.Dot(dir_n)) {
			max_dot = enemyBeamDir.Dot(dir_n);
			m_pEnemy = (*itr);
		}
	}
}

void CBeamHoming::Update(){
	if (m_pEnemy != NULL) {
		CVector3D dirT = m_start_target_pos - m_pos;
		dirT.Normalize();
		CVector3D dirE = m_pEnemy->m_pos - m_pos;
		dirE.Normalize();
		if (dirT.Dot(dirE) > 0.0f || dirT.Dot(dirE) < 0.0f) {
			CVector3D dir_n = m_pEnemy->m_pos - target_pos;
			dir_n.Normalize();
			target_pos += dir_n * m_df;
		}
	}
	CVector3D fwd(0.0f, 0.0f, 1.0f);
	m_rotMtx.RotationAxis(fwd, m_pos, target_pos);
	CBeam::Update();
}
