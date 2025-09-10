#pragma once

#include "CGameModelX.h"
#include "CEffect.h"
#include "CBeam.h"

class CEnemy : public CGameModelX {
public:
	bool m_alive;
	bool m_Stop;
	CVector3D *m_pTargetPos;
	CVector3D m_TargetPos;
	int m_fb;
	CEnemy() :m_alive(true),m_Stop(false), m_pTargetPos(0), m_fb(ENEMY_FB) {
		
	}
	void Init() {
		Load("sazabi.x");
	}
	void Move(CVector4D dir);

	void Update();
	void Update_Shot_Beam();
	void UpdateMatrix();
	void Render();
};
