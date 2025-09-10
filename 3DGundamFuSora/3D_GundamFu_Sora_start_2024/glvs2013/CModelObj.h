#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "glew.h"
#include "glut.h"
#include "CTexture.h"
#include "CModel.h"
class CMaterialObj {
public:
	float m_ambient[4];
	float m_diffuse[4];
	float m_specular[4];
	float m_shininess;
	CTexture *m_pTex;

public:
	CMaterialObj();
	void Map();
	void Unmap();
	void Release();
};

class CModelObj : public CModel {
private:
protected:
public:
	CVector3D*	m_pVertexAry;
	CVector3D*	m_pNormalAry;
	CVector2D*	m_pTexCoordAry;
	int			m_vertexNum;
	CMaterialObj	m_material;
	

	bool	LoadMaterial(char *path);
public:
	
	CModelObj();
	~CModelObj();
	bool	Load(const char *path);
	void	RenderM(CMatrix &m);
	void	Release();

};