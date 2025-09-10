#include "CPlayer.h"
#include "CBeamManager.h"
#include "CBeamHoming.h"

void CPlayer::Shot() {
	fb--;
	if (fb < 0) {
		if (::GetKeyState(' ') & 0x80) { //’e
			CBeam *b = new CBeam(CBeam::SBT::PLAYER);
			b->InitBoxMesh(0.03125, 0.03125, 1.0f);
			b->Start(GetBeamPos(), GetBeamPos() + CVector3D(0.0f, 0.0f, BEAM_RANGE) * m_rotMtx);
			CBeamManager::GetInstance()->Add(b);
		}

		fb = PLAYER_FB;
	}
}

CVector3D CPlayer::GetBeamPos() {
	CVector4D beam(-0.85f, 0.4f, 2.0f, 1.0f);
	CVector3D vec3;
	beam = m_matrix * beam;
	vec3.x = beam.x;
	vec3.y = beam.y;
	vec3.z = beam.z;
	return vec3;
}

void CPlayer::UpdateMatrix(){
	CMatrix RX, RY;
	RX.RotationX(m_rot.x);
	RY.RotationY(m_rot.y);
	m_rotMtx = m_rotMtx * RX;
	m_rotMtx = m_rotMtx * RY;
	m_transMtx.Transelate(m_pos.x, m_pos.y, m_pos.z);
	m_scaleMtx.Scale(m_scale.x, m_scale.y, m_scale.z);
	m_matrix = m_transMtx * m_rotMtx * m_scaleMtx;
	m_rot.x = 0.0f;
	m_rot.y = 0.0f;
}

void CPlayer::Update() {
	UpdateMatrix();

	m_leftVernier.Update();
	m_rightVernier.Update();
}

void CPlayer::Render() {
	CGameModelX::Render();
	m_leftVernier.Render(m_matrix);
	m_rightVernier.Render(m_matrix);
}
