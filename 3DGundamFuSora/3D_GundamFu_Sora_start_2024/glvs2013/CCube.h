#pragma once

#include "CMatrix.h"
#include "CVector.h"
#include "CShader.h"

class CCube{
	SVector3D*	m_pVertexArray;
	SVector3D*	m_pNormalArray;
	int			m_vertexNum;
	CMatrix*	m_pMatrixArray;
	GLuint		m_buffers[3];
	int*		m_boneIndexArray;
	CShader		m_shader;
public:
	CCube();
	~CCube();
	void draw();
};