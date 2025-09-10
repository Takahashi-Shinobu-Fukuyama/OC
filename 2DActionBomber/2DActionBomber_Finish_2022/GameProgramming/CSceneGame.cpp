#include "CSceneGame.h"
#include "CBlock.h"
#include "CBackGround.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "Define.h"

std::vector<CCharacter*> CSceneGame::mCharacters;

CTexture TexBomberman;

//���\�b�h�i�v���O�����j�̒�`
int cols = 0, rows = 0;

CSceneGame::CSceneGame()
{
	TexBomberman.Load("Bomberman.tga");
	int map[] = {
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9 ,
		 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 9 ,
		 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 9 ,
		 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9 ,
		 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 9,
		 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 9,
		 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 9,
		 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9,
		 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 9,
		 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 9,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10,
	};
	int col = 1;
	for (int i = 0; map[i] < 10; i++) {
		int x;
		int y;
		if (map[i] == 9)
		{ 
			if (col == 1)
			{
				col = i + 1;
			}
		}
		else {
			if (col == 1)
			{
				x = (i) * CHIPSIZE * 2 + CHIPSIZE;
				y = -1 * CHIPSIZE * 2 + CHIPSIZE;
			}
			else
			{
				x = (i % col) * CHIPSIZE * 2 + CHIPSIZE;
				y = (-i / col - 1) * CHIPSIZE * 2 + CHIPSIZE;
			}
			if (cols < x)
			{
				cols = x;
			}
			if (rows > y)
			{
				rows = y;
			}
			//�}�b�v�̍쐬
			switch (map[i]) {
			case 1:		// 1�̎��̓u���b�N��z�u
				//�u���b�N�̔z�u
				new CBlock(x, y, CHIPSIZE, CHIPSIZE);
				break;
				//1�@�}�b�v�̍쐬
			default:
				new CBackGround(x, y, CHIPSIZE, CHIPSIZE);
			}
		}
	}
	for (int i = 0; map[i] < 10; i++) {
		int x;
		int y;
		x = (i % col) * CHIPSIZE * 2 + CHIPSIZE;
		y = (-i / col - 1) * CHIPSIZE * 2 + CHIPSIZE;
		if (map[i] < 9)
		{
			//2�@�v���C���[�̔z�u
			if (map[i] == 2) {
				new CPlayer(x, y, CHIPSIZE, CHIPSIZE);
			}
			//7�@�G�̔z�u
			if (map[i] == 3) {
				new CEnemy(x, y, CHIPSIZE, CHIPSIZE);
			}
		}
	}
	
//	mCamera.Camera2D(400.0f, -300.0f, 400.0f, 300.0f);
	cols += CHIPSIZE;
	rows -= CHIPSIZE;
	cols /= 2;
	rows /= 2;
}

CSceneGame::~CSceneGame() {
	//�S�ẴL�����폜
	std::vector<CCharacter*>::iterator itr = mCharacters.begin();
	while (itr != mCharacters.end()) {
		delete *itr;
		itr = mCharacters.erase(itr);
	}
}


void CSceneGame::Init() {
}

void CSceneGame::Update() {
	//�X�V����
	for (int i = 0; i < mCharacters.size(); i++) {
		mCharacters[i]->Update();
	}
	//�Փˏ���
	for (int i = 0; i < mCharacters.size(); i++) {
		for (int j = i + 1; j < mCharacters.size(); j++) {
			mCharacters[i]->Collision(mCharacters[i], mCharacters[j]);
			mCharacters[j]->Collision(mCharacters[j], mCharacters[i]);
		}
	}
	//�����ȃL�����폜
	std::vector<CCharacter*>::iterator itr = mCharacters.begin();
	while ( itr != mCharacters.end() ) {
		if ((*itr)->mState == CCharacter::EENABLED) {
			itr++;
		}
		else {
			delete *itr;
			itr = mCharacters.erase(itr);
		}
	}
}

void CSceneGame::Render() {
	mCamera.Camera2D(cols, rows, cols, -rows);
	//�`�揈��
	for (int i = 0; i < mCharacters.size(); i++) {
		mCharacters[i]->Render();
	}
}
