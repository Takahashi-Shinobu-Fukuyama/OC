#include <Windows.h>
#include "CSceneGame.h"
#include "CParticle.h"
#include "CXPlayer.h"
#include "CXEnemyKnight.h"
#include "glut.h"

#define MODEL_FILE "ラグナ.x"	//入力ファイル名
#define MODEL_KNIGHT "Data\\knight\\knight_low.X"

void CSceneGame::Init() {
	//プレイヤーのモデルの読み込み
	mModel.Load(MODEL_FILE);
	//敵のモデル読み込み
	mModelKnight.Load(MODEL_KNIGHT);
	//アニメーションの分割
	mModelKnight.SeparateAnimationSet(10, 80, "walk");//1:移動
	mModelKnight.SeparateAnimationSet(1530, 1830, "idle1");//2:待機
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(440, 520, "attack1");//7:Attack1
	mModelKnight.SeparateAnimationSet(520, 615, "attack2");//8:Attack2
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(10, 80, "walk");
	mModelKnight.SeparateAnimationSet(1160, 1260, "death1");//11:ダウン
	//ヒットエフェクト画像の読み込み
	CEffectHit2::mTexEffectHit.load("Data\\ken5L.tga");
	CEffectHit3::mTexEffectHit.load("Data\\ken14L.tga");

	//*2
	//プレイヤーの生成
	new CXPlayer(&mModel, CVector3(-10.0f, 5.0f, -65.0f));
	//*9
	//敵の生成
	new CXEnemyKnight(&mModelKnight, CVector3(0.0f, 0.0f, -50.0f));
	//*12
	//敵の生成
	new CXEnemyKnight(&mModelKnight, CVector3(-10.0f, 0.0f, -40.0f));
}

void CSceneGame::Update() {
	//タスクの更新
	CTaskManager::Get()->Update();
	//コリジョンマネージャの更新
	CCollisionManager::Get()->Update();
	//無効なタスクの削除
	CTaskManager::Get()->Remove();
	//描画
	Render();
}

void CSceneGame::Render() {
	//カメラ設定
	if (CXPlayer::mpPlayer)
		mCamera.update(CXPlayer::mpPlayer->GetCameraPosition(), CXPlayer::mpPlayer->GetTargetPosition(), CVector3(0.0f, 1.0f, 0.0f));
	//*1
	//マップの描画
	mMap.Render();
	//タスクの描画
	CTaskManager::Get()->Render();
}

