#pragma once
#include "stdio.h"
#include "CVector3.h"

class CLightBase {
private:
	CVector3 m_lightDir;
	CVector3 m_AmbientColor;
	CVector3 m_DiffuseColor;
public:
	void setDir(CVector3 v){
		m_lightDir = v;
	}
	void setColor(CVector3 a,CVector3 d){
		m_AmbientColor = a;
		m_DiffuseColor = d;
	}
	CVector3 getDir(){
		return m_lightDir;
	}
	CVector3 getAmbientColor(){
		return m_AmbientColor;
	}
	CVector3 getDiffuseColor(){
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