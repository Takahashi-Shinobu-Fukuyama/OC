#include "CRectangle.h"

CRectangle::CRectangle(float x, float y, float z, float halfWidth, float halfHeight)
:m_centralPosition(x,y,z), m_halfWidth(halfWidth), m_halfHeight(halfHeight) {
	CreateRectangle(x, y, z, halfWidth, halfHeight);
}

void CRectangle::CreateRectangle(float x, float y, float z, float halfWidth, float halfHeight){
	CVector3D vertexAray[] = {
		CVector3D(x - halfWidth, y + halfHeight, z),
		CVector3D(x + halfWidth, y - halfHeight, z),
		CVector3D(x + halfWidth, y + halfHeight, z),
		CVector3D(x - halfWidth, y + halfHeight, z),
		CVector3D(x - halfWidth, y - halfHeight, z),
		CVector3D(x + halfWidth, y - halfHeight, z),
	};
	CVector3D normalAray[] = {
		CVector3D(0.0f, 0.0f, 1.0f),
		CVector3D(0.0f, 0.0f, 1.0f),
		CVector3D(0.0f, 0.0f, 1.0f),
		CVector3D(0.0f, 0.0f, 1.0f),
		CVector3D(0.0f, 0.0f, 1.0f),
		CVector3D(0.0f, 0.0f, 1.0f),
	};
}

