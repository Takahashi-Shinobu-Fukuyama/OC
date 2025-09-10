#pragma once
#include "Define.h"
#include "CGameModelX.h"
#include "CParticleSystem.h"

class CPlayer : public CGameModelX {
	int fb;
	CParticleSystem m_leftVernier, m_rightVernier;
	CModelObj m_particleBlueModel;
public:
	CPlayer() : fb(0) {};
	CVector3D GetBeamPos();
	void Shot();
	void UpdateMatrix();

	void Init()	{
		m_particleBlueModel.Load("particle2.obj");
		//m_rightVernier.Init(CVector3D(-0.3f, 0.18f, -0.18f), PARTICLE_NUM, &m_particleBlueModel, PARTICLE_SCALE, PARTICLE_MOVE_SPEED, PARTICLE_MAXLIFE, 0.005f);
		//m_leftVernier.Init(CVector3D(0.18f, 0.18f, -0.18f), PARTICLE_NUM, &m_particleBlueModel, PARTICLE_SCALE, PARTICLE_MOVE_SPEED, PARTICLE_MAXLIFE, 0.005f);
		m_rightVernier.Init(CVector3D(-0.3f, 0.18f, -0.18f), PARTICLE_NUM, &m_particleBlueModel, PARTICLE_SCALE, 0.0015f, 20, 0.001f);
		m_leftVernier.Init(CVector3D(0.18f, 0.18f, -0.18f), PARTICLE_NUM, &m_particleBlueModel, PARTICLE_SCALE, 0.0015f, 20, 0.001f);
		m_leftVernier.Start(true);
		m_rightVernier.Start(true);
	};
	void Update();
	void Render();

};
