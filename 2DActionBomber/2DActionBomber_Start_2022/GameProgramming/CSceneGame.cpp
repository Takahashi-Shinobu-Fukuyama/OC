#include "CSceneGame.h"
#include "CBlock.h"
#include "CBackGround.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "Define.h"

std::vector<CCharacter*> CSceneGame::mCharacters;

CTexture TexBomberman;
int cols = 0, rows = 0;

//���\�b�h�i�v���O�����j�̒�`
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

			}
		}
	}
	for (int i = 0; map[i] < 10; i++) {
		int x = (i % col) * CHIPSIZE * 2 + CHIPSIZE;
		int y = (-i / col - 1) * CHIPSIZE * 2 + CHIPSIZE;
		if (map[i] < 9)
		{
			//2�@�v���C���[�̔z�u

			//7�@�G�̔z�u

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
	mCamera.Camera2D(cols, rows, cols, -rows);

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
	//�`�揈��
	for (int i = 0; i < mCharacters.size(); i++) {
		mCharacters[i]->Render();
	}
}
