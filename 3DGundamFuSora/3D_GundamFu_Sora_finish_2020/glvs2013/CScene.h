#pragma once

#include "CMatrix.h"
#include "CPlayer.h"

#include "CModelObj.h"
#include "CEnemy.h"
#include "CEffect.h"
#include "CBeam.h"

#include "CEG.h"
#include "CUIBar.h"

#include "CImage2.h"

#include "CParticleSystem.h"

class CScene {
private:
	CVector4D c_pos; //カメラの移動距離
	CVector3D c_rot; //カメラの回転

	CEffect m_Effect;

	int status; //シーンの種別

	CUIBar m_enemyDamageUI;
	CUIBar m_playerDamageUI;

	CImage2 m_ImageLose;
	CImage2 m_ImageWin;

	int m_EndTime;

public:

	static CPlayer m_player; //プレイヤー

	static CModelObj m_Space[2];

	CParticleSystem m_particleSpark;
	CParticleSystem m_particleExplosion;

	CModelObj m_particleYellowModel;

	CScene() :status(2), c_pos(0, 0, 10.0, 1), c_rot(0.0, 0, 0),
		m_enemyDamageUI(0.0f, 0.0f, 0.0f, 20.0f, 1.0f, 1.0f, 0.0f, 0.5f),
		m_playerDamageUI(0.0f, 20.0f, 0.0f, 20.0f, 1.0f, 0.4f, 0.4f, 0.5f){
	};
	~CScene() {
	};

	void Stage1Init();
	void Stage1Render();
	void Stage1Update();
	void Lose();
	void Win();
	void ReStart();

	void Update();
	void Render();

	void CollisionEnemyBeam();
	void CollisionPlayerBeam();
	bool key(char c);

	void m1_PlayerRender();
	void m2_EnemyLocate();
	void m2_EnemyGenerate();
	CVector4D getPlayerTranslate();
	void m3_PlayerTransform();
	void m4_PlayerShotBeam();
	void m5_CollisionEnemyBeam();
	void m6_EnemyUpdate();
	void m7_CollisionPlayerBeam();
	void m8_Effect();
	void m9_UIRender();
	void m10_Result();
};
