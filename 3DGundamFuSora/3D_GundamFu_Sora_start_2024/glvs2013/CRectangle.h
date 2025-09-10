#pragma once
#include "CModelObj.h"
#include "CVector.h"

class CRectangle : CModelObj {
private:
	CVector3D  m_centralPosition;
	float m_halfWidth;
	float m_halfHeight;

public:
	CRectangle(float x, float y, float z, float halfWidth, float halfHeight);
	void CreateRectangle(float x, float y, float z, float halfWidth, float halfHeight);

};