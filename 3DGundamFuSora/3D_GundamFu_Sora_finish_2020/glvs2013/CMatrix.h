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
	//単位行列を設定
	void SetIndentity();
	//平行移動行列を設定
	void Transelate(float x,float y, float z);
	//スケール行列を設定
	void Scale(float x,float y, float z);
	//クオータニオン→行列
	void setQuaternion(float x,float y, float z,float w);
	//回転行列を設定
	void RotationX(float r);
	void RotationY(float r);
	void RotationZ(float r);
	//任意軸で回転行列を設定
	void RotationAxis(float vx,float vy,float vz,float r);
	//カメラ行列用
	void LookAt(const CVector3D& eye,const CVector3D& at,const CVector3D& up);
	//逆行列を取得
	CMatrix getInverse( void );

	//各種演算
	CMatrix operator +(const CMatrix& num);
	CMatrix operator *(CMatrix num);
	CMatrix operator *(const float num);


	CVector4D operator *(const CVector4D num);
	CVector3D operator *(const CVector3D num);
	/**
	 @brief 現在位置から目的地へ向く回転行列を求める設定する
	 @param[in]	fwd	現在の向き（ベクトル）
	 @param[in]	pos 現在位置（ベクトル）
	 @param[in]	target	目的地（ベクトル）
	*/
	void RotationAxis(const CVector3D& fwd, const CVector3D& pos, const CVector3D& target) {
		CVector3D diff = target - pos;
		diff.Normalize();
		CVector3D normal = fwd.Cross(diff);
		normal.Normalize();
		//真後ろ？
		if (fwd.Dot(diff) < -0.9999f) {
			RotationAxis(0.0f, 1.0f, 0.0f, acos(-1.0f));
		}
		//真正面以外？
		else if (fwd.Dot(diff) < 0.9999f) {
			RotationAxis(normal.x, normal.y, normal.z, acos(diff.Dot(fwd)));
		}

	}


};
CMatrix operator *(const float num,const CMatrix mtr);
CVector4D operator * (const CVector4D num,const CMatrix mtr);
CVector3D operator * (const CVector3D num,const CMatrix mtr);