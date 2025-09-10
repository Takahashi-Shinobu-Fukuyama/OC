#pragma once

#include "CMatrix.h"

class CModel {
protected:
public:
	CVector3D m_pos;
	CVector3D m_rot;
	CVector3D m_scale;

	CMatrix m_matrix;
	CMatrix m_transMtx;
	CMatrix m_rotMtx;
	CMatrix m_scaleMtx;
public:
	CModel() : m_pos(0,0,0),m_rot(0,0,0),m_scale(1,1,1) {
	}
	void setPos(CVector3D pos){
		m_pos = pos;
	}
	void setPos(float x,float y,float z){
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	}	
	CVector3D getPos(){
		return m_pos;
	}
	void setRot(CVector3D rot){
		m_rot = rot;
	}
	void setRot(float x,float y,float z){
		m_rot.x = x;
		m_rot.y = y;
		m_rot.z = z;
	}	
	CVector3D getRot(){
		return m_rot;
	}
	void setScale(CVector3D scale){
		m_scale = scale;
	}
	void setScale(float x,float y,float z){
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}	
	CVector3D getScale(){
		return m_scale;
	}
	CMatrix getTransMatrix() {
		return m_transMtx;
	}
	CMatrix getRotMatrix() {
		return m_rotMtx;
	}
	CMatrix getScaleMatrix() {
		return m_scaleMtx;
	}

	virtual void UpdateMatrix() {
		CMatrix RX,RY,RZ;
		RX.RotationX(m_rot.x);
		RY.RotationY(m_rot.y);
		RZ.RotationZ(m_rot.z);
		m_rotMtx = RZ * RY * RX;
		m_transMtx.Transelate(m_pos.x, m_pos.y, m_pos.z);
		m_scaleMtx.Scale(m_scale.x,m_scale.y,m_scale.z);
		m_matrix = m_transMtx * m_rotMtx * m_scaleMtx;
	}
	/*
	  çsóÒÇê›íË
	*/
	void setMatrix(CMatrix &m){
		m_matrix = m;
	}
	CMatrix getMatrix(){
		return m_matrix;
	}

	virtual bool Load(const char *path)=0;
	virtual void RenderM(CMatrix &m)=0;
	virtual void Render(){
		UpdateMatrix();
		RenderM(m_matrix);
	}
	virtual void Release()=0;


};