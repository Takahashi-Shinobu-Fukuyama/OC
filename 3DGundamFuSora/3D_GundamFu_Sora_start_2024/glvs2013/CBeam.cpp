#include "CBeam.h"
#include "CVector.h"
#include "CCollision.h"
#include "CBeamManager.h"
#include "CEBeamManager.h"
#include <windows.h>

//ビームの寿命 大きいほど寿命が早い
#define BEAM_LIFE_TIME 0.04f

CBeam::CBeam() : target_pos(0.0f, 0.0f, 0.0f), m_alive(false), sx(0.0f), sy(0.0f), sz(0.0f), m_trail(0.0f)
{
	//	InitBoxMesh(1.0f,1.0f, 1.0f);
}
CBeam::CBeam(SBT s) : target_pos(0.0f, 0.0f, 0.0f), m_alive(false), sx(0.0f), sy(0.0f), sz(0.0f), m_trail(0.0f), sbt(s)
{
	//	InitBoxMesh(1.0f,1.0f, 1.0f);
}
void CBeam::Start(const CVector3D& pos, const CVector3D& target) {
	CVector3D fwd(0.0f, 0.0f, 1.0f);
	m_pos = pos;
	target_pos = target;
	m_alive = true;

	m_rotMtx.RotationAxis(fwd, m_pos, target_pos);

	m_life = BEAM_RANGE;
}

void CBeam::Render() {
	if (m_alive)
		CModelObj::Render();
}


void CBeam::InitBoxMesh(float sx, float sy, float sz) {
	InitBoxMesh(sx, sy, sz, 1.0f, 1.0f, 0.0f, 1.0f);
}
void CBeam::InitBoxMesh(float sx, float sy, float sz, float r, float g, float b, float a)
{
	CVector3D pos(0,0,0);
	CVector3D len(sx, sy, sz);
	this->sx = sx;
	this->sy = sy;
	this->sz = sz;

	int FaceNum = 12;
	m_vertexNum = 3*FaceNum;

	//描画用頂点配列を作成
	m_pVertexAry = new CVector3D[m_vertexNum];
	//描画用法線配列を作成
	m_pNormalAry = new CVector3D[m_vertexNum];

	m_pVertexAry[0] = CVector3D(pos.x - len.x, pos.y + len.y, pos.z + len.z);
	m_pVertexAry[1] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z + len.z);
	m_pVertexAry[2] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z + len.z);
	m_pNormalAry[0] = m_pNormalAry[1] = m_pNormalAry[2] = CVector3D(0.0f, 0.0f, 1.0f);

	m_pVertexAry[3] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z + len.z);
	m_pVertexAry[4] = CVector3D(pos.x + len.x, pos.y - len.y, pos.z + len.z);
	m_pVertexAry[5] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z + len.z);
	m_pNormalAry[3] = m_pNormalAry[4] = m_pNormalAry[5] = CVector3D(0.0f, 0.0f, 1.0f);

	m_pVertexAry[6] = CVector3D(pos.x - len.x, pos.y + len.y, pos.z - len.z);
	m_pVertexAry[7] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z - len.z);
	m_pVertexAry[8] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z - len.z);
	m_pNormalAry[6] = m_pNormalAry[7] = m_pNormalAry[8] = CVector3D(0.0f, 0.0f, -1.0f);

	m_pVertexAry[9] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z - len.z);
	m_pVertexAry[10] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z - len.z);
	m_pVertexAry[11] = CVector3D(pos.x + len.x, pos.y - len.y, pos.z - len.z);
	m_pNormalAry[9] = m_pNormalAry[10] = m_pNormalAry[11] = CVector3D(0.0f, 0.0f, -1.0f);

	m_pVertexAry[12] = CVector3D(pos.x - len.x, pos.y + len.y, pos.z - len.z);
	m_pVertexAry[13] = CVector3D(pos.x - len.x, pos.y + len.y, pos.z + len.z);
	m_pVertexAry[14] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z + len.z);
	m_pNormalAry[12] = m_pNormalAry[13] = m_pNormalAry[14] = CVector3D(0.0f, 1.0f, 0.0f);

	m_pVertexAry[15] = CVector3D(pos.x - len.x, pos.y + len.y, pos.z - len.z);
	m_pVertexAry[16] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z + len.z);
	m_pVertexAry[17] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z - len.z);
	m_pNormalAry[15] = m_pNormalAry[16] = m_pNormalAry[17] = CVector3D(0.0f, 1.0f, 0.0f);

	m_pVertexAry[20] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z - len.z);
	m_pVertexAry[19] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z + len.z);
	m_pVertexAry[18] = CVector3D(pos.x + len.x, pos.y - len.y, pos.z + len.z);
	m_pNormalAry[18] = m_pNormalAry[19] = m_pNormalAry[20] = CVector3D(0.0f, -1.0f, 0.0f);

	m_pVertexAry[23] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z - len.z);
	m_pVertexAry[22] = CVector3D(pos.x + len.x, pos.y - len.y, pos.z + len.z);
	m_pVertexAry[21] = CVector3D(pos.x + len.x, pos.y - len.y, pos.z - len.z);
	m_pNormalAry[21] = m_pNormalAry[22] = m_pNormalAry[23] = CVector3D(0.0f, -1.0f, 0.0f);

	m_pVertexAry[24] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z + len.z);
	m_pVertexAry[25] = CVector3D(pos.x + len.x, pos.y - len.y, pos.z + len.z);
	m_pVertexAry[26] = CVector3D(pos.x + len.x, pos.y - len.y, pos.z - len.z);
	m_pNormalAry[24] = m_pNormalAry[25] = m_pNormalAry[26] = CVector3D(1.0f, 0.0f, 0.0f);

	m_pVertexAry[27] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z + len.z);
	m_pVertexAry[28] = CVector3D(pos.x + len.x, pos.y - len.y, pos.z - len.z);
	m_pVertexAry[29] = CVector3D(pos.x + len.x, pos.y + len.y, pos.z - len.z);
	m_pNormalAry[27] = m_pNormalAry[28] = m_pNormalAry[29] = CVector3D(1.0f, 0.0f, 0.0f);

	m_pVertexAry[32] = CVector3D(pos.x - len.x, pos.y + len.y, pos.z + len.z);
	m_pVertexAry[31] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z + len.z);
	m_pVertexAry[30] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z - len.z);
	m_pNormalAry[30] = m_pNormalAry[31] = m_pNormalAry[32] = CVector3D(-1.0f, 0.0f, 0.0f);

	m_pVertexAry[35] = CVector3D(pos.x - len.x, pos.y + len.y, pos.z + len.z);
	m_pVertexAry[34] = CVector3D(pos.x - len.x, pos.y - len.y, pos.z - len.z);
	m_pVertexAry[33] = CVector3D(pos.x - len.x, pos.y + len.y, pos.z - len.z);
	m_pNormalAry[33] = m_pNormalAry[34] = m_pNormalAry[35] = CVector3D(-1.0f, 0.0f, 0.0f);

	m_material.m_diffuse[0] = r;
	m_material.m_diffuse[1] = g;
	m_material.m_diffuse[2] = b;
	m_material.m_diffuse[3] = a;

	m_material.m_ambient[0] = 0.0f;
	m_material.m_ambient[1] = 0.0f;
	m_material.m_ambient[2] = 0.0f;
	m_material.m_ambient[3] = 0.0f;
	
	m_material.m_specular[0] = r;
	m_material.m_specular[1] = g;
	m_material.m_specular[2] = b;
	m_material.m_specular[3] = a;

	m_material.m_shininess = 0.0f;
}

void CBeam::Update()
{
	if (!m_alive)
		return;

	if (m_trail <= 0.0f) {
  		CBeam *b = new CBeam();
		*b = *this;
		b->setArray(*this);
		b->m_trail = BEAM_LIFE_TIME;
		switch (b->sbt) {
		case CBeam::SBT::PLAYER:
			CBeamManager::GetInstance()->Add(b);
			break;
		case CBeam::SBT::ENEMY:
			CEBeamManager::GetInstance()->Add(b);
			break;
		}
		CVector3D diff = target_pos - m_pos;
		/*
		if (diff.Length() < 1.0f) {
			m_alive = false;
			return;
		}
		*/
		diff.Normalize();
		m_pos += diff * BEAM_SPEED;
		m_life -= 1.0f * BEAM_SPEED;
		if (m_life < 0.0f) {
			m_alive = false;
			return;
		}
	}
	else
	{
		m_material.m_diffuse[3] -= m_trail;
		if (m_material.m_diffuse[3] <= 0.0f) {
			m_alive = false;
		}
	}
}

void CBeam::UpdateMatrix()
{

	CMatrix translate;
	translate.SetIndentity();
	translate.m03 = m_pos.x;
	translate.m13 = m_pos.y;
	translate.m23 = m_pos.z;

	m_matrix = translate * m_rotMtx;
}

void CBeam::setDiffuse(float r, float g, float b, float a) {
	m_material.m_diffuse[0] = r;
	m_material.m_diffuse[1] = g;
	m_material.m_diffuse[2] = b;
	m_material.m_diffuse[3] = a;
}

/*
bool CBeam::IntersectTriangleRay(CVector3D *corss,const CVector3D &p1, const CVector3D &p2,float *pt ) {
	for(int i = 0; i < m_vertexNum; i+=3) {
		if(CCollision::IntersectTriangleRay(corss, p1, p2, m_pVertexAry[i], m_pVertexAry[i+1], m_pVertexAry[i+2],pt)) {
			return true;
		}
	}
	return false;
}
*/

float* CBeam::getDiffuse() {
	return m_material.m_diffuse;
}
/*
CVector3D CBeam::GetEuler(CVector3D &v) {
	float rx, ry, rz;
	rx = atan2f(v.y, v.Dot(CVector3D(1.0f, 0.0f, 0.0f)));
	ry = atan2f(v.z, v.x);
	rz = 0.0f;
	if (v.x >= 0.0f) {
		return CVector3D(rx, -ry, rz);
	}
	else {
		if (v.y >= 0.0f) {
			return CVector3D(rx, ry, rz);
			if (v.z >= 0.0f) {

			}
		}
		else {
			return CVector3D(rx, ry, rz);
		}
	}
}
*/

//代入演算子
CBeam& CBeam::setArray(const CBeam& b) {
//	m_vertexNum = b.m_vertexNum;
	//描画用頂点配列を作成
 	m_pVertexAry = new CVector3D[m_vertexNum];
	//描画用法線配列を作成
	m_pNormalAry = new CVector3D[m_vertexNum];

	memcpy(m_pVertexAry, b.m_pVertexAry, sizeof(CVector3D) * m_vertexNum);
	memcpy(m_pNormalAry, b.m_pNormalAry, sizeof(CVector3D) * m_vertexNum);

//	m_material = b.m_material;
//	m_alive = b.m_alive;

	return(*this);
}
