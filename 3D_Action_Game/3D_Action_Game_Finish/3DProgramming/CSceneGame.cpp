#include <Windows.h>
#include "CSceneGame.h"
#include "CParticle.h"
#include "CXPlayer.h"
#include "CXEnemyKnight.h"
#include "glut.h"

#define MODEL_FILE "���O�i.x"	//���̓t�@�C����
#define MODEL_KNIGHT "Data\\knight\\knight_low.X"

void CSceneGame::Init() {
	//�v���C���[�̃��f���̓ǂݍ���
	mModel.Load(MODEL_FILE);
	//�G�̃��f���ǂݍ���
	mModelKnight.Load(MODEL_KNIGHT);
	//�A�j���[�V�����̕���
	mModelKnight.SeparateAnimationSet(10, 80, "walk");//1:�ړ�
	mModelKnight.SeparateAnimationSet(1530, 1830, "idle1");//2:�ҋ@
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(440, 520, "attack1");//7:Attack1
	mModelKnight.SeparateAnimationSet(520, 615, "attack2");//8:Attack2
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(1160, 1260, "death1");//11:�_�E��
	//�q�b�g�G�t�F�N�g�摜�̓ǂݍ���
	CEffectHit2::mTexEffectHit.load("Data\\ken5L.tga");
	CEffectHit3::mTexEffectHit.load("Data\\ken14L.tga");

	//*2
	//�v���C���[�̐���
	new CXPlayer(&mModel, CVector3(-10.0f, 5.0f, -65.0f));
	//*9
	//�G�̐���
	new CXEnemyKnight(&mModelKnight, CVector3(0.0f, 0.0f, -50.0f));
	//*12
	//�G�̐���
	new CXEnemyKnight(&mModelKnight, CVector3(-10.0f, 0.0f, -40.0f));
}

void CSceneGame::Update() {
	//�^�X�N�̍X�V
	CTaskManager::Get()->Update();
	//�R���W�����}�l�[�W���̍X�V
	CCollisionManager::Get()->Update();
	//�����ȃ^�X�N�̍폜
	CTaskManager::Get()->Remove();
	//�`��
	Render();
}

void CSceneGame::Render() {
	//�J�����ݒ�
	if (CXPlayer::mpPlayer)
		mCamera.update(CXPlayer::mpPlayer->GetCameraPosition(), CXPlayer::mpPlayer->GetTargetPosition(), CVector3(0.0f, 1.0f, 0.0f));
	//*1
	//�}�b�v�̕`��
	mMap.Render();
	//�^�X�N�̕`��
	CTaskManager::Get()->Render();
}

