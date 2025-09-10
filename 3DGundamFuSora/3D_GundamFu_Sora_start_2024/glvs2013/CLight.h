#pragma once
#include "stdio.h"
#include "CVector.h"

class CLightBase {
private:
	CVector3D m_lightDir;
	CVector3D m_AmbientColor;
	CVector3D m_DiffuseColor;
public:
	void setDir(CVector3D v){
		m_lightDir = v;
	}
	void setColor(CVector3D a,CVector3D d){
		m_AmbientColor = a;
		m_DiffuseColor = d;
	}
	CVector3D getDir(){
		return m_lightDir;
	}
	CVector3D getAmbientColor(){
		return m_AmbientColor;
	}
	CVector3D getDiffuseColor(){
		return m_DiffuseColor;
	}

};
class CLight {
public:
	static const int LIGHT_MAX = 5;
private:
	static CLightBase *m_light;
public:
	static CLightBase* getLight(int no) {
		if(m_light==NULL) m_light = new CLightBase[LIGHT_MAX];
		return &m_light[no];
	}

};