#pragma once

#include "CModelObj.h"

class CBillboard : public CModelObj {
public:
	void Update();
	void Render();
//	int VertexCopy(CVector3D vertex[], CVector3D* normal, CVector2D* texcorrd);
	static void GetBillboardMatrix(CMatrix *out, const CVector3D &pos, const CVector3D &scale);
};
