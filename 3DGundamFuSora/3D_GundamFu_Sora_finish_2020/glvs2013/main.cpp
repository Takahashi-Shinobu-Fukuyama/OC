#define _USE_MATH_DEFINES
#define FULL_SCREEN 0

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "glew.h"
#include "glut.h"

#include "CMatrix.h"

#include "CModelX.h"
#include "CModelSX.h"
#include "CTexture.h"
#include "CXLoader.h"
#include "CVector.h"
#include "CCube.h"
#include "CLight.h"
#include "CCamera.h"
#include "CSound.h"
#include "CKey.h"
#include "CImage.h"
#include "CCollision.h"
#include "CModelObj.h"
#include "CFPS.h"

#include "CScene.h"

CScene scene;
//CScene scene;

void display(void) {
	//各バッファーをクリア
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//行列の初期化
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//-------------------------------------
//    キャラクターの移動
//-------------------------------------
	//プレイヤーの位置
//	static CVector3D pos(0,0,0);
	//プレイヤーの回転値
//	static float a = M_PI;
/*
	//移動中フラグ
	bool isMove=false;
	//重力落下速度
	static float vec_y = 0;
	static float speed = 0;
	//ジャンプ
	if(CKey::getState(0,CKey::eHold,CKey::eButton1)) {
		vec_y=0.8f;
	}
	//前移動
	if(CKey::getState(0,CKey::eHold,CKey::eUp)) {
		speed+=0.03f;
		if(speed>1.0) speed = 1.0f; 
		isMove = true;
	} else {
		speed-=0.05f;
		if(speed<0) speed = 0.00f;
	
	}
	pos+=CVector3D(sin(a)*speed*0.6,0,cos(a)*speed*0.6);

	//左回転
	if(CKey::getState(0,CKey::eHold,CKey::eLeft)) {
		a+=0.1f;
	}
	//右回転
	if(CKey::getState(0,CKey::eHold,CKey::eRight)) {
		a-=0.1f;
	}
	//落下
	pos.y+=vec_y;
	vec_y-=0.03f;
*/
//
//----------------------------------------------------


/*
//-------------------------------------------------------------
//ポリゴン（マップ）とカプセル（キャラクター）の判定
//-------------------------------------------------------------
	//衝突判定用キャラの大きさ（半径）
	#define COL_RUDIUS 5.0f
	//結果格納用
	CCollTriangle hit[32];
	//マップモデルとカプセルの判定
	int num = sm.CollisionCupsel(hit,CVector3D(pos.x,pos.y+COL_RUDIUS+10.0,pos.z),CVector3D(pos.x,pos.y+COL_RUDIUS+0.5f,pos.z),COL_RUDIUS);
	//衝突したポリゴンの数を表示
	printf("num = %d\n",num);
	//衝突したポリゴンの数だけループ
	for(int i=0;i<num;i++) {
		//めり込んだ分、ポリゴンの法線方向へ押し戻す
		pos.x += hit[i].m_normal.x*(COL_RUDIUS-hit[i].m_dist);
		pos.z += hit[i].m_normal.z*(COL_RUDIUS-hit[i].m_dist);
		pos.y += hit[i].m_normal.y*(COL_RUDIUS-hit[i].m_dist);
		//地面だったら傾きに応じて重力落下速度を減らす
		if(hit[i].m_normal.y>0) vec_y*=1-hit[i].m_normal.y;
		//printf("dist = %08.2f ny = %08.2f\n",hit[i].m_dist,hit[i].m_normal.y);
	}
//
//---------------------------------------------------

*/

//
//----------------------------------------------------

//---------------------------------------------------------
//	カメラ
//---------------------------------------------------------
/*	{
		#define CAM_SPEED 0.2
		static CVector4D c_pos(0 , 0 , -5.0 , 1);
		static CVector3D c_rot(0.3 , 0 , 0);

		//キーによる回転と移動
		if(::GetKeyState('J') & 0x80) {
			c_rot.y+=0.05f;
		}
		if(::GetKeyState('L') & 0x80) {
			c_rot.y-=0.05f;
		}
		if(::GetKeyState('I') & 0x80) {
			c_rot.x+=0.05f;
		}
		if(::GetKeyState('K') & 0x80) {
			c_rot.x-=0.05f;
		}
		if(::GetKeyState('U') & 0x80) {
			c_pos.z-=CAM_SPEED;
		}
		if(::GetKeyState('O') & 0x80) {
			c_pos.z+=CAM_SPEED;
		}
		//カメラの回転値から回転行列を作成
		CMatrix mRX,mRY,m;
		mRX.RotationX(c_rot.x);
		mRY.RotationY(c_rot.y);
		m = mRY*mRX;
		
		//移動ベクトルを計算
		CVector4D c_p= c_pos*m;
		
		//注視カメラ設定
		//カメラの位置　キャラクターからc_p分移動した座標
		//注視点　キャラクターの座標
		CCamera::getCamera()->LookAt(CVector3D(pos.x+c_p.x,pos.y+0.0+c_p.y,pos.z+c_p.z),
								CVector3D(pos.x, pos.y, pos.z),
								CVector3D(0.0, 1.0, 0.0));

	}
*/

//
//-------------------------------------------
	//ゲームプログラム更新
	scene.Update();

	//カメラ行列を反映
	glLoadMatrixf(CCamera::getCamera()->getMatrix().f);

//---------------------------------------
// 表示
//---------------------------------------
	scene.Render();

//
//-----------------------------------------

	glutSwapBuffers();

}

void init (void) 
{
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
	CKey::setButton(0,CKey::eButton1,'Z');
	CKey::setButton(0,CKey::eButton2,'X');
	CKey::setButton(0,CKey::eButton3,'C');
	CKey::setButton(0,CKey::eButton4,'V');
	CKey::setButton(0,CKey::eUp,VK_UP);
	CKey::setButton(0,CKey::eDown,VK_DOWN);
	CKey::setButton(0,CKey::eLeft,VK_LEFT);
	CKey::setButton(0,CKey::eRight,VK_RIGHT);

}


void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	CCamera::getCamera()->SetSize(w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, (double)w / (double)h, 1.0, 1000.0);

}
void idle(void)
{
	CKey::upDate();
	display();
	
	CFPS::Wait();
	
	char title[32];
	sprintf_s(title,"sample %d",CFPS::getFPS());
	glutSetWindowTitle(title);

}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '\033':  // '\033' は ESC の ASCII コード
		exit(0);
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
#if (FULL_SCREEN)
	glutGameModeString("640x480:32@60");
	glutEnterGameMode();
#else
	glutInitWindowSize (1024, 768); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("sample");
#endif
	glewInit();
	
	SetCurrentDirectory("data");

	glClearColor (1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);

//	glEnable(GL_BLEND);//ブレンドの有効化

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	
	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//メッシュシェーダー用
	CLight::getLight(0)->setDir(CVector3D(0,-1,1).GetNormalize());
	CLight::getLight(0)->setColor(CVector3D(0.9f,0.9f,0.9f),CVector3D(1.0f,1.0f,1.0f));
		
	init ();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);

	scene.Update();

	glutMainLoop();

	return 0;
}