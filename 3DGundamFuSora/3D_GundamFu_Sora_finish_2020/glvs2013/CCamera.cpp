#include "CCamera.h"

CCamera CCamera::m_camera[5];
CCamera* CCamera::m_pCurrent = &m_camera[0];


//カメラオブジェクトを取得
CCamera* CCamera::getCamera(int num) {
	return &m_camera[num];
}
//カメラオブジェクトを取得
CCamera* CCamera::getCurrent() {
	return m_pCurrent;
}
//カメラオブジェクトを取得
void CCamera::setCurrent(int num) {
	m_pCurrent = &m_camera[num];
}
//カメラオブジェクトを取得
void CCamera::setCurrent(CCamera *p) {
	m_pCurrent = p;
}
//コンストラクタ
CCamera::CCamera(){
}
//幅を取得
float CCamera::getWhidth() {
	return m_whidth;
}
//高さを取得
float CCamera::getHeight() {
	return m_height;
}
//幅と高さを設定
void CCamera::SetSize(float w,float h) {
	m_whidth = w;
	m_height = h;
}
//注視カメラでビュー行列を設定
void CCamera::LookAt(CVector3D &eye,CVector3D &at,CVector3D &up){
	m_pos = eye;
	m_at = at;
	m_up = up;
	m_matrix.LookAt(eye,at,up);
	m_dir = CVector3D(-m_matrix.m20,-m_matrix.m21,-m_matrix.m22);
}
//位置と回転値でビュー行列を設定
void CCamera::setTranseRot(CVector3D &pos,CVector3D &rot){
	//カメラの回転行列を作成
	CMatrix mRT,mRX,mRY,mRZ,m;
	mRX.RotationX(rot.x);
	mRY.RotationY(rot.y+3.14);
	mRZ.RotationZ(rot.z);
	mRT.Transelate(pos.x,pos.y,pos.z);
	m = mRZ*mRY*mRX;
	CVector3D r(m.m00,m.m10,m.m20);
	CVector3D u(m.m01,m.m11,m.m21);
	CVector3D f(m.m02,m.m12,m.m22);
	//カメラ行列の倒置行列からビュー変換行列を作成
	m_matrix = CMatrix(	m.m00,m.m10,m.m20,-(pos.x * m.m00 + pos.y * m.m10 + pos.z * m.m20),
						m.m01,m.m11,m.m21,-(pos.x * m.m01 + pos.y * m.m11 + pos.z * m.m21),
						m.m02,m.m12,m.m22,-(pos.x * m.m02 + pos.y * m.m12 + pos.z * m.m22),
						0,0,0,1);
	//カメラの向きを取得
	m_dir = CVector3D(-m.m02,-m.m12,-m.m22);
}
//ビュー行列を直接設定
void CCamera::setMatrix(CMatrix &m) {
	m_matrix = m;
}
//ビュー行列を取得
CMatrix CCamera::getMatrix(){
	return m_matrix;
}
//カメラの位置を取得
CVector3D CCamera::getPos() {
	return m_pos;
}
//カメラの向きを取得
CVector3D CCamera::getDir() {
	return m_dir;
}