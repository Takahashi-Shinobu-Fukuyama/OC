#pragma once

#include "CVector.h"
#include "CMatrix.h"
class CCamera {
private:
	CVector3D m_pos;	//カメラの位置
	CVector3D m_at;		//注視点
	CVector3D m_up;		//Upベクトル
	CVector3D m_dir;	//向き（Frontベクトル）
	CMatrix m_matrix;	//カメラ行列
	float	m_whidth;	//カメラの幅
	float	m_height;	//　〃　　高さ
	static CCamera m_camera[5];
	static CCamera *m_pCurrent;
public:
	//カメラオブジェクトを取得
	static CCamera* getCamera(int num=0);
	//カメラオブジェクトを取得
	static CCamera* getCurrent();
	static void setCurrent(int num);
	static void setCurrent(CCamera *p);
	//コンストラクタ
	CCamera();
	//幅を取得
	float getWhidth();
	//高さを取得
	float getHeight();
	//幅と高さを設定
	void SetSize(float w,float h);
	//注視カメラでビュー行列を設定
	void LookAt(CVector3D &eye,CVector3D &at,CVector3D &up);
	//位置と回転値でビュー行列を設定
	void setTranseRot(CVector3D &pos,CVector3D &rot);
	//ビュー行列を直接設定
	void setMatrix(CMatrix &m);
	//ビュー行列を取得
	CMatrix getMatrix();
	//カメラの位置を取得
	CVector3D getPos();
	//カメラの向きを取得
	CVector3D getDir();

	//カメラの上方向を取得
	CVector3D getUp() { return m_up; };
};