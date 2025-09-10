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
	case 0:	//�Z���N�g��ʏ�����
//		SelectInit();
		status = 1;
		break;
	case 1:	//�Z���N�g��ʍX�V
//		SelectUpdate();
		break;
	case 2:	//�X�e�[�W�P������
		Stage1Init();
		status = 3;
		break;
	case 3:	//�X�e�[�W�P�X�V
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

//�`�揈��
void CScene::Render()
{
	switch(status) {
	case 1:	//�Z���N�g��ʕ`��
//		SelectRender();
		break;
	case 3:	//�X�e�[�W�P�`��
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

//�X�e�[�W�P������
void CScene::Stage1Init()
{
	//�v���C���[�쐬
	m_player.Load("gundam3.s.x");

	//�F���쐬
	m_Space[0].Load("HalfSphere.obj");
	m_Space[1].Load("HalfSphere.obj");
	m_Space[0].setRot(0.0, M_PI, 0.0f);
	float s = 3.0f;
	m_Space[0].setScale(s, s, s);
	m_Space[1].setScale(s, s, s);

	//Enemy Generator ������
	CEG::Init();

	//�G�̔z�u
	m2_EnemyLocate();

	//�G�t�F�N�g������
	m_Effect.Init();

	m_ImageLose.Load("youlose.png");
	m_ImageWin.Load("youwin.png");

	m_EndTime = timeGetTime() + 60*1000;

	//�p�[�e�B�N���V�X�e������
//	m_particleBlueModel.Load("particle2.obj");
	m_particleYellowModel.Load("particle.obj");
	//CParticleSystem::init(pos, num, objfile, scale, moveSpeed,maxlife, rangeRadius)
	m_particleSpark.Init(CVector3D(-2.0f, 0.0f, 0.0f), 300, &m_particleYellowModel, 0.10f, 0.1f, 30, 0.5f);
	m_particleExplosion.Init(CVector3D(-2.0f, 0.0f, 0.0f), 100, &m_particleYellowModel, 0.6f, 0.05f, 40, 0.0f);

	m_player.Init();
}

//�X�e�[�W�P�`��
void CScene::Stage1Render()
{
	//�w�i�\��
	m_Space[0].Render();
	m_Space[1].Render();

	//�G�\��
	CEnemyManager::GetInstance()->Render();

	//�r�[���\��
	CBeamManager::GetInstance()->Render();
	CEBeamManager::GetInstance()->Render();

	//�v���C���[�\��
	m1_PlayerRender();

	m8_Effect();

	//UI�\��
	m9_UIRender();
}

CVector4D CScene::getPlayerTranslate() {
	CVector4D dir(0.0f, 0.0f, 0.0f, 1.0f);
	//�v���C���[�̓��͂��󂯕t����
	if (key('A')) { //��
		dir.x = PLAYER_TRAN;
	}
	if (key('W')) { //��
		dir.z = PLAYER_TRAN;
	}
	if (key('S')) { //��
		dir.z -= PLAYER_TRAN;
	}
	if (key('D')) { //�E
		dir.x = -PLAYER_TRAN;
	}
	return dir;
}

//�X�e�[�W�P�X�V
void CScene::Stage1Update() {
	CVector4D dir(0.0f, 0.0f, 0.0f,1.0f);

	//�G�̏o��
	m2_EnemyGenerate();

	//�v���C���[��]
	m3_PlayerTransform();
	//�v���C���[�O�㍶�E�ړ�
	dir = getPlayerTranslate();

	//�v���C���[�r�[������
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

	//�J�����̈ړ��x�N�g�����v�Z
	CVector4D c_positionShift = CVector4D(0.0f, 2.0f, -5.0f, 1.0f) * m_player.m_rotMtx;
	CVector4D c_top = CVector4D(0.0f, 1.0f, 0.0f, 1.0f) * m_player.m_rotMtx;

	//�����J�����ݒ�
	//�J�����̈ʒu�@�L�����N�^�[����c_p���ړ��������W
	//�����_�@�L�����N�^�[�̍��W
	CCamera::getCamera()->LookAt(
		CVector3D(m_player.m_pos.x + c_positionShift.x, m_player.m_pos.y + 0.0 + c_positionShift.y, m_player.m_pos.z + c_positionShift.z),
		CVector3D(m_player.m_pos.x, m_player.m_pos.y, m_player.m_pos.z),
		CVector3D(c_top.x, c_top.y, c_top.z));


	//�G�t�F�N�g�X�V
	CEffectManager::GetInstance()->Update();
	CParticleManager::GetInstance()->Update();

	//�r�[���X�V
	CBeamManager::GetInstance()->Update();
	CEBeamManager::GetInstance()->Update();

	//�G�ƃr�[���̓����蔻��
	m5_CollisionEnemyBeam();

	//�v���C���[�ƓG�r�[���̓����蔻��
	m7_CollisionPlayerBeam();

	//�G�̍X�V
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

//�G�ƃr�[���̓����蔻��
void CScene::CollisionEnemyBeam() {
	std::list<CEnemy*> array = CEnemyManager::GetInstance()->GetArray();
	std::list<CEnemy*>::iterator itr;
	for (itr = array.begin(); itr != array.end(); itr++) {
		CBeam* beam = CBeamManager::GetInstance()->Collision((*itr));
		if (beam != NULL) {
			/**/
			//�G�t�F�N�g�쐬
			CEffect* e = new CEffect();
			*e = m_Effect;
			e->m_pos = beam->m_pos;
			e->SetColor(beam->m_material.m_diffuse[0], beam->m_material.m_diffuse[1], beam->m_material.m_diffuse[2]);
			e->Start();
			CEffectManager::GetInstance()->Add(e);
			/**/

			/**/
			//�X�p�[�N�p�[�e�B�N���쐬
			CParticleSystem* c = new CParticleSystem();
			c->Copy(m_particleSpark);
			c->m_pos = beam->m_pos;
			c->Start();
			CParticleManager::GetInstance()->Add(c);
			//�����p�[�e�B�N���쐬
			c = new CParticleSystem();
			c->Copy(m_particleExplosion);
			c->m_pos = beam->m_pos;
			c->Start();
			CParticleManager::GetInstance()->Add(c);
			/**/
			beam->m_alive = false;
			//Enemy��~
			(*itr)->m_Stop = true;
			m_enemyDamageUI.setPoint(m_enemyDamageUI.getPoint() + 100);
			if (m_enemyDamageUI.getPoint() > CCamera::getCurrent()->getWhidth()) {
				status = 5;
			}
		}
	}
}

//�v���C���[�ƓG�r�[���̓����蔻��
void CScene::CollisionPlayerBeam() {
	CBeam* beam = CEBeamManager::GetInstance()->Collision(&CScene::m_player);
	if (beam != NULL) {
		/**/
		//�X�p�[�N�p�[�e�B�N���쐬
		CParticleSystem* c = new CParticleSystem();
		c->Copy(m_particleSpark);
		c->m_pos = beam->m_pos;
		c->Start();
		CParticleManager::GetInstance()->Add(c);
		/**/
		/**/
		//�G�t�F�N�g�쐬
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
	//�v���C���[�\��
}

void CScene::m2_EnemyLocate() {
	//�G�̔z�u
}

void CScene::m2_EnemyGenerate() {
	//�G�̏o��
}

void CScene::m3_PlayerTransform() {
	//�L�[�ɂ���]
}

void CScene::m4_PlayerShotBeam() {
	//�v���C���[�r�[������
}

void CScene::m5_CollisionEnemyBeam() {
	//�G�ƃr�[���̓����蔻��
}

void CScene::m6_EnemyUpdate() {
	//�G�̍X�V
}

void CScene::m7_CollisionPlayerBeam() {
	//�v���C���[�ƓG�r�[���̓����蔻��
}

void CScene::m8_Effect() {
	//�G�t�F�N�g�\��
}

void CScene::m9_UIRender() {
	//UI�\��
}

void CScene::m10_Result() {
}
