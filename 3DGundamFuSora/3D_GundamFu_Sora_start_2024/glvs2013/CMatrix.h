#pragma once
#include <string.h>
#include <math.h>
#include "CVector.h"
struct SMatrix
{
	union
	{
		float	f[16];
		float	m[4][4];
		struct
		{
			float	m00,m10,m20,m30,
					m01,m11,m21,m31,
					m02,m12,m22,m32,
					m03,m13,m23,m33;
		};
	};
};
class CMatrix : public SMatrix {
public:
	CMatrix() {
		SetIndentity();
	}
	CMatrix(float mat00,float mat01,float mat02,float mat03,
			float mat10,float mat11,float mat12,float mat13,
			float mat20,float mat21,float mat22,float mat23,
			float mat30,float mat31,float mat32,float mat33) {
				m00 = mat00; m01 = mat01; m02 = mat02; m03 = mat03;
				m10 = mat10; m11 = mat11; m12 = mat12; m13 = mat13;
				m20 = mat20; m21 = mat21; m22 = mat22; m23 = mat23;
				m30 = mat30; m31 = mat31; m32 = mat32; m33 = mat33;
	}
	//�P�ʍs���ݒ�
	void SetIndentity();
	//���s�ړ��s���ݒ�
	void Transelate(float x,float y, float z);
	//�X�P�[���s���ݒ�
	void Scale(float x,float y, float z);
	//�N�I�[�^�j�I�����s��
	void setQuaternion(float x,float y, float z,float w);
	//��]�s���ݒ�
	void RotationX(float r);
	void RotationY(float r);
	void RotationZ(float r);
	//�C�ӎ��ŉ�]�s���ݒ�
	void RotationAxis(float vx,float vy,float vz,float r);
	//�J�����s��p
	void LookAt(const CVector3D& eye,const CVector3D& at,const CVector3D& up);
	//�t�s����擾
	CMatrix getInverse( void );

	//�e�퉉�Z
	CMatrix operator +(const CMatrix& num);
	CMatrix operator *(CMatrix num);
	CMatrix operator *(const float num);


	CVector4D operator *(const CVector4D num);
	CVector3D operator *(const CVector3D num);
	/**
	 @brief ���݈ʒu����ړI�n�֌�����]�s������߂�ݒ肷��
	 @param[in]	fwd	���݂̌����i�x�N�g���j
	 @param[in]	pos ���݈ʒu�i�x�N�g���j
	 @param[in]	target	�ړI�n�i�x�N�g���j
	*/
	void RotationAxis(const CVector3D& fwd, const CVector3D& pos, const CVector3D& target) {
		CVector3D diff = target - pos;
		diff.Normalize();
		CVector3D normal = fwd.Cross(diff);
		normal.Normalize();
		//�^���H
		if (fwd.Dot(diff) < -0.9999f) {
			RotationAxis(0.0f, 1.0f, 0.0f, acos(-1.0f));
		}
		//�^���ʈȊO�H
		else if (fwd.Dot(diff) < 0.9999f) {
			RotationAxis(normal.x, normal.y, normal.z, acos(diff.Dot(fwd)));
		}

	}


};
CMatrix operator *(const float num,const CMatrix mtr);
CVector4D operator * (const CVector4D num,const CMatrix mtr);
CVector3D operator * (const CVector3D num,const CMatrix mtr);