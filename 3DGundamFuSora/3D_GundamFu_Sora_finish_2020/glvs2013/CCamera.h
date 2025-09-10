#pragma once

#include "CVector.h"
#include "CMatrix.h"
class CCamera {
private:
	CVector3D m_pos;	//�J�����̈ʒu
	CVector3D m_at;		//�����_
	CVector3D m_up;		//Up�x�N�g��
	CVector3D m_dir;	//�����iFront�x�N�g���j
	CMatrix m_matrix;	//�J�����s��
	float	m_whidth;	//�J�����̕�
	float	m_height;	//�@�V�@�@����
	static CCamera m_camera[5];
	static CCamera *m_pCurrent;
public:
	//�J�����I�u�W�F�N�g���擾
	static CCamera* getCamera(int num=0);
	//�J�����I�u�W�F�N�g���擾
	static CCamera* getCurrent();
	static void setCurrent(int num);
	static void setCurrent(CCamera *p);
	//�R���X�g���N�^
	CCamera();
	//�����擾
	float getWhidth();
	//�������擾
	float getHeight();
	//���ƍ�����ݒ�
	void SetSize(float w,float h);
	//�����J�����Ńr���[�s���ݒ�
	void LookAt(CVector3D &eye,CVector3D &at,CVector3D &up);
	//�ʒu�Ɖ�]�l�Ńr���[�s���ݒ�
	void setTranseRot(CVector3D &pos,CVector3D &rot);
	//�r���[�s��𒼐ڐݒ�
	void setMatrix(CMatrix &m);
	//�r���[�s����擾
	CMatrix getMatrix();
	//�J�����̈ʒu���擾
	CVector3D getPos();
	//�J�����̌������擾
	CVector3D getDir();

	//�J�����̏�������擾
	CVector3D getUp() { return m_up; };
};