#include "CEnemy.h"
#include "CCollision.h"
#include "CScene.h"
#include "CEBeamManager.h"


void CEnemy::Move(CVector4D dir) {
	m_pos.x -= dir.x;
	m_pos.y -= dir.y;
	m_pos.z -= dir.z;
}

void CEnemy::Update() {
	if (!m_alive)
		return;
//	if (m_pTargetPos == NULL){
//		m_pTargetPos = &CScene::m_player.m_pos;
//	}
//	CVector3D diff = *m_pTargetPos - m_pos;
	CVector3D diff = m_TargetPos - m_pos;
	if (diff.Length() < 5.0f) {
		m_TargetPos = CScene::m_player.m_pos;
		m_TargetPos += diff * 5;
		return;
	}
	diff.Normalize();
	m_pos += diff * ENEMY_TRAN;

	m_rotMtx.RotationAxis(CVector3D(0.0f, 0.0f, 1.0f), m_pos, m_TargetPos);

	Update_Shot_Beam();
}

void CEnemy::Update_Shot_Beam() {
	m_fb--;
	if (m_fb < 0) {
		CVector3D diff = CScene::m_player.getPos() - m_pos;
		diff.Normalize();

		if (diff.Dot(CVector3D(0.0f, 0.0f, 1.0f) * m_rotMtx) > 0.0) {
			CBeam *b = new CBeam(CBeam::SBT::ENEMY);
			//		b->InitBoxMesh(0.03125, 0.03125, 1.0f, 1.0f, 0.4f, 0.4f, 1.0f);
			b->InitBoxMesh(0.03125, 0.03125, 1.0f, 1.0f, 0.4f, 0.4f, 1.0f);
			CMatrix m;
			m.RotationAxis(CVector3D(0.0f, 0.0f, 1.0f), m_pos, CScene::m_player.getPos());
//			b->Start(m_pos, m_pos + CVector3D(0.0f, 0.0f, BEAM_RANGE) * m_rotMtx);
			b->Start(m_pos, m_pos + CVector3D(0.0f, 0.0f, BEAM_RANGE) * m);
			CEBeamManager::GetInstance()->Add(b);
			m_fb = ENEMY_FB;
		}
	}
}

void CEnemy::UpdateMatrix()
{

	CMatrix translate;
	translate.SetIndentity();
	translate.m03 = m_pos.x;
	translate.m13 = m_pos.y;
	translate.m23 = m_pos.z;

	m_matrix = translate * m_rotMtx;
}

void CEnemy::Render() {
	if (m_alive)
		CModelX::Render();
}