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
	//�e�o�b�t�@�[���N���A
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//�s��̏�����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//-------------------------------------
//    �L�����N�^�[�̈ړ�
//-------------------------------------
	//�v���C���[�̈ʒu
//	static CVector3D pos(0,0,0);
	//�v���C���[�̉�]�l
//	static float a = M_PI;
/*
	//�ړ����t���O
	bool isMove=false;
	//�d�͗������x
	static float vec_y = 0;
	static float speed = 0;
	//�W�����v
	if(CKey::getState(0,CKey::eHold,CKey::eButton1)) {
		vec_y=0.8f;
	}
	//�O�ړ�
	if(CKey::getState(0,CKey::eHold,CKey::eUp)) {
		speed+=0.03f;
		if(speed>1.0) speed = 1.0f; 
		isMove = true;
	} else {
		speed-=0.05f;
		if(speed<0) speed = 0.00f;
	
	}
	pos+=CVector3D(sin(a)*speed*0.6,0,cos(a)*speed*0.6);

	//����]
	if(CKey::getState(0,CKey::eHold,CKey::eLeft)) {
		a+=0.1f;
	}
	//�E��]
	if(CKey::getState(0,CKey::eHold,CKey::eRight)) {
		a-=0.1f;
	}
	//����
	pos.y+=vec_y;
	vec_y-=0.03f;
*/
//
//----------------------------------------------------


/*
//-------------------------------------------------------------
//�|���S���i�}�b�v�j�ƃJ�v�Z���i�L�����N�^�[�j�̔���
//-------------------------------------------------------------
	//�Փ˔���p�L�����̑傫���i���a�j
	#define COL_RUDIUS 5.0f
	//���ʊi�[�p
	CCollTriangle hit[32];
	//�}�b�v���f���ƃJ�v�Z���̔���
	int num = sm.CollisionCupsel(hit,CVector3D(pos.x,pos.y+COL_RUDIUS+10.0,pos.z),CVector3D(pos.x,pos.y+COL_RUDIUS+0.5f,pos.z),COL_RUDIUS);
	//�Փ˂����|���S���̐���\��
	printf("num = %d\n",num);
	//�Փ˂����|���S���̐��������[�v
	for(int i=0;i<num;i++) {
		//�߂荞�񂾕��A�|���S���̖@�������։����߂�
		pos.x += hit[i].m_normal.x*(COL_RUDIUS-hit[i].m_dist);
		pos.z += hit[i].m_normal.z*(COL_RUDIUS-hit[i].m_dist);
		pos.y += hit[i].m_normal.y*(COL_RUDIUS-hit[i].m_dist);
		//�n�ʂ�������X���ɉ����ďd�͗������x�����炷
		if(hit[i].m_normal.y>0) vec_y*=1-hit[i].m_normal.y;
		//printf("dist = %08.2f ny = %08.2f\n",hit[i].m_dist,hit[i].m_normal.y);
	}
//
//---------------------------------------------------

*/

//
//----------------------------------------------------

//---------------------------------------------------------
//	�J����
//---------------------------------------------------------
/*	{
		#define CAM_SPEED 0.2
		static CVector4D c_pos(0 , 0 , -5.0 , 1);
		static CVector3D c_rot(0.3 , 0 , 0);

		//�L�[�ɂ���]�ƈړ�
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
		//�J�����̉�]�l�����]�s����쐬
		CMatrix mRX,mRY,m;
		mRX.RotationX(c_rot.x);
		mRY.RotationY(c_rot.y);
		m = mRY*mRX;
		
		//�ړ��x�N�g�����v�Z
		CVector4D c_p= c_pos*m;
		
		//�����J�����ݒ�
		//�J�����̈ʒu�@�L�����N�^�[����c_p���ړ��������W
		//�����_�@�L�����N�^�[�̍��W
		CCamera::getCamera()->LookAt(CVector3D(pos.x+c_p.x,pos.y+0.0+c_p.y,pos.z+c_p.z),
								CVector3D(pos.x, pos.y, pos.z),
								CVector3D(0.0, 1.0, 0.0));

	}
*/

//
//-------------------------------------------
	//�Q�[���v���O�����X�V
	scene.Update();

	//�J�����s��𔽉f
	glLoadMatrixf(CCamera::getCamera()->getMatrix().f);

//---------------------------------------
// �\��
//---------------------------------------
	scene.Render();

//
//-----------------------------------------

	glutSwapBuffers();

}

void init (void) 
{
	//�t���[�����䏉����
	CFPS::Init();
	//�{�^���̐ݒ�
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
	case '\033':  // '\033' �� ESC �� ASCII �R�[�h
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

//	glEnable(GL_BLEND);//�u�����h�̗L����

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	
	//�Œ�V�F�[�_�[�p
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//���b�V���V�F�[�_�[�p
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