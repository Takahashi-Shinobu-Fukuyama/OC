#include "Define.h"
#include "CScene.h"
#include "CCollision.h"
#include <windows.h>
#include "CCamera.h"
#include "CBeamManager.h"
#include "CEBeamManager.h"
#include "CEffectManager.h"
#include "CEnemyManager.h"
#include "CParticleManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

CPlayer CScene::m_player;
CModelObj CScene::m_Space[2];


void CScene::Update()
{
	switch(status) {
	case 0:	//セレクト画面初期化
//		SelectInit();
		status = 1;
		break;
	case 1:	//セレクト画面更新
//		SelectUpdate();
		break;
	case 2:	//ステージ１初期化
		Stage1Init();
		status = 3;
		break;
	case 3:	//ステージ１更新
		Stage1Update();
		break;
	case 4:
	case 5:
		if (key('Z')) {
			status = 6;
		}
		break;
	case 6:
		ReStart();
		status = 3;
		m_EndTime = timeGetTime() + 60 * 1000;
		break;
	}
}

void CScene::ReStart() {
	CEBeamManager::GetInstance()->Destroy();
	CBeamManager::GetInstance()->Destroy();
//	CEnemyManager::GetInstance()->Destroy();
	CParticleManager::GetInstance()->Destroy();
	m_playerDamageUI.setPoint(0.0f);
	m_enemyDamageUI.setPoint(0.0f);
}

//描画処理
void CScene::Render()
{
	switch(status) {
	case 1:	//セレクト画面描画
//		SelectRender();
		break;
	case 3:	//ステージ１描画
		Stage1Render();
		break;
	case 4:
		m10_Result();
		break;
	case 5:
		m10_Result();
		break;
	}
}

void CScene::Lose(){
	Stage1Render();
	m_ImageLose.DrawCenter(0, 0, 315, 72);
}

void CScene::Win(){
	Stage1Render();
	m_ImageWin.DrawCenter(0, 0, 315, 72);
}

//ステージ１初期化
void CScene::Stage1Init()
{
	//プレイヤー作成
	m_player.Load("gundam3.s.x");

	//宇宙作成
	m_Space[0].Load("HalfSphere.obj");
	m_Space[1].Load("HalfSphere.obj");
	m_Space[0].setRot(0.0, M_PI, 0.0f);
	float s = 3.0f;
	m_Space[0].setScale(s, s, s);
	m_Space[1].setScale(s, s, s);

	//Enemy Generator 初期化
	CEG::Init();

	//敵の配置
	m2_EnemyLocate();

	//エフェクト初期化
	m_Effect.Init();

	m_ImageLose.Load("youlose.png");
	m_ImageWin.Load("youwin.png");

	m_EndTime = timeGetTime() + 60*1000;

	//パーティクルシステム準備
//	m_particleBlueModel.Load("particle2.obj");
	m_particleYellowModel.Load("particle.obj");
	//CParticleSystem::init(pos, num, objfile, scale, moveSpeed,maxlife, rangeRadius)
	m_particleSpark.Init(CVector3D(-2.0f, 0.0f, 0.0f), 300, &m_particleYellowModel, 0.10f, 0.1f, 30, 0.5f);
	m_particleExplosion.Init(CVector3D(-2.0f, 0.0f, 0.0f), 100, &m_particleYellowModel, 0.6f, 0.05f, 40, 0.0f);

	m_player.Init();
}

//ステージ１描画
void CScene::Stage1Render()
{
	//背景表示
	m_Space[0].Render();
	m_Space[1].Render();

	//敵表示
	CEnemyManager::GetInstance()->Render();

	//ビーム表示
	CBeamManager::GetInstance()->Render();
	CEBeamManager::GetInstance()->Render();

	//プレイヤー表示
	m1_PlayerRender();

	m8_Effect();

	//UI表示
	m9_UIRender();
}

CVector4D CScene::getPlayerTranslate() {
	CVector4D dir(0.0f, 0.0f, 0.0f, 1.0f);
	//プレイヤーの入力を受け付ける
	if (key('A')) { //左
		dir.x = PLAYER_TRAN;
	}
	if (key('W')) { //上
		dir.z = PLAYER_TRAN;
	}
	if (key('S')) { //下
		dir.z -= PLAYER_TRAN;
	}
	if (key('D')) { //右
		dir.x = -PLAYER_TRAN;
	}
	return dir;
}

//ステージ１更新
void CScene::Stage1Update() {
	CVector4D dir(0.0f, 0.0f, 0.0f,1.0f);

	//敵の出現
	m2_EnemyGenerate();

	//プレイヤー回転
	m3_PlayerTransform();
	//プレイヤー前後左右移動
	dir = getPlayerTranslate();

	//プレイヤービーム発射
	m4_PlayerShotBeam();

	dir = dir * m_player.m_rotMtx;

	m_player.m_pos.x += dir.x;
	m_player.m_pos.y += dir.y;
	m_player.m_pos.z += dir.z;

	m_player.Update();

	for (int i = 0; i < 2; i++) {
		CVector3D pos = m_Space[i].getPos();
		pos.x += dir.x;
		pos.y += dir.y;
		pos.z += dir.z;
		m_Space[i].setPos(pos);
	}

	//カメラの移動ベクトルを計算
	CVector4D c_positionShift = CVector4D(0.0f, 2.0f, -5.0f, 1.0f) * m_player.m_rotMtx;
	CVector4D c_top = CVector4D(0.0f, 1.0f, 0.0f, 1.0f) * m_player.m_rotMtx;

	//注視カメラ設定
	//カメラの位置　キャラクターからc_p分移動した座標
	//注視点　キャラクターの座標
	CCamera::getCamera()->LookAt(
		CVector3D(m_player.m_pos.x + c_positionShift.x, m_player.m_pos.y + 0.0 + c_positionShift.y, m_player.m_pos.z + c_positionShift.z),
		CVector3D(m_player.m_pos.x, m_player.m_pos.y, m_player.m_pos.z),
		CVector3D(c_top.x, c_top.y, c_top.z));


	//エフェクト更新
	CEffectManager::GetInstance()->Update();
	CParticleManager::GetInstance()->Update();

	//ビーム更新
	CBeamManager::GetInstance()->Update();
	CEBeamManager::GetInstance()->Update();

	//敵とビームの当たり判定
	m5_CollisionEnemyBeam();

	//プレイヤーと敵ビームの当たり判定
	m7_CollisionPlayerBeam();

	//敵の更新
	m6_EnemyUpdate();

	if (m_EndTime < timeGetTime()) {
		if (m_enemyDamageUI.getPoint() < m_playerDamageUI.getPoint()) {
			status = 4;
		}
		else {
			status = 5;
		}
	}

}

//敵とビームの当たり判定
void CScene::CollisionEnemyBeam() {
	std::list<CEnemy*> array = CEnemyManager::GetInstance()->GetArray();
	std::list<CEnemy*>::iterator itr;
	for (itr = array.begin(); itr != array.end(); itr++) {
		CBeam* beam = CBeamManager::GetInstance()->Collision((*itr));
		if (beam != NULL) {
			/**/
			//エフェクト作成
			CEffect* e = new CEffect();
			*e = m_Effect;
			e->m_pos = beam->m_pos;
			e->SetColor(beam->m_material.m_diffuse[0], beam->m_material.m_diffuse[1], beam->m_material.m_diffuse[2]);
			e->Start();
			CEffectManager::GetInstance()->Add(e);
			/**/

			/**/
			//スパークパーティクル作成
			CParticleSystem* c = new CParticleSystem();
			c->Copy(m_particleSpark);
			c->m_pos = beam->m_pos;
			c->Start();
			CParticleManager::GetInstance()->Add(c);
			//爆発パーティクル作成
			c = new CParticleSystem();
			c->Copy(m_particleExplosion);
			c->m_pos = beam->m_pos;
			c->Start();
			CParticleManager::GetInstance()->Add(c);
			/**/
			beam->m_alive = false;
			//Enemy停止
			(*itr)->m_Stop = true;
			m_enemyDamageUI.setPoint(m_enemyDamageUI.getPoint() + 100);
			if (m_enemyDamageUI.getPoint() > CCamera::getCurrent()->getWhidth()) {
				status = 5;
			}
		}
	}
}

//プレイヤーと敵ビームの当たり判定
void CScene::CollisionPlayerBeam() {
	CBeam* beam = CEBeamManager::GetInstance()->Collision(&CScene::m_player);
	if (beam != NULL) {
		/**/
		//スパークパーティクル作成
		CParticleSystem* c = new CParticleSystem();
		c->Copy(m_particleSpark);
		c->m_pos = beam->m_pos;
		c->Start();
		CParticleManager::GetInstance()->Add(c);
		/**/
		/**/
		//エフェクト作成
		CEffect* e = new CEffect();
		*e = m_Effect;
		e->m_pos = beam->m_pos;
		e->SetColor(beam->m_material.m_diffuse[0], beam->m_material.m_diffuse[1], beam->m_material.m_diffuse[2]);
		e->Start();
		CEffectManager::GetInstance()->Add(e);
		/**/
		beam->m_alive = false;
		m_playerDamageUI.setPoint(m_playerDamageUI.getPoint() + 100);
		if (m_playerDamageUI.getPoint() > CCamera::getCurrent()->getWhidth()) {
			status = 4;
		}
	}
}

bool CScene::key(char c) {
	return ::GetKeyState(c) & 0x80;
}

void CScene::m1_PlayerRender() {
	//プレイヤー表示
}

void CScene::m2_EnemyLocate() {
	//敵の配置
}

void CScene::m2_EnemyGenerate() {
	//敵の出現
}

void CScene::m3_PlayerTransform() {
	//キーによる回転
}

void CScene::m4_PlayerShotBeam() {
	//プレイヤービーム発射
}

void CScene::m5_CollisionEnemyBeam() {
	//敵とビームの当たり判定
}

void CScene::m6_EnemyUpdate() {
	//敵の更新
}

void CScene::m7_CollisionPlayerBeam() {
	//プレイヤーと敵ビームの当たり判定
}

void CScene::m8_Effect() {
	//エフェクト表示
}

void CScene::m9_UIRender() {
	//UI表示
}

void CScene::m10_Result() {
}
