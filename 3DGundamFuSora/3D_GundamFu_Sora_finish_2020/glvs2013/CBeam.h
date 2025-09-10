#pragma once
#include "Define.h"
#include "CModelObj.h"

class CBeam : public CModelObj
{
public:
	enum SBT {
		PLAYER,
		ENEMY
	};
	float sx;	//Xサイズ
	float sy;	//Yサイズ
	float sz;	//Zサイズ
	bool m_alive;	//true:有効  false:無効
	CVector3D target_pos; //移動場所
	float m_trail;
	SBT sbt;
	float m_life;
	CBeam();
	CBeam(SBT s);
	void InitBoxMesh(float sx, float sy, float sz);
	void InitBoxMesh(float sx, float sy, float sz, float r, float g, float b, float a);
	virtual void Update();
	void UpdateMatrix();
	void setDiffuse(float r, float g, float b, float a);
	//線分との衝突判定
	bool IntersectTriangleRay(CVector3D *corss,const CVector3D &p1, const CVector3D &p2,float *pt );
	float* getDiffuse();
	virtual void Start(const CVector3D& pos, const CVector3D& target);
	void Render();
	CVector3D GetEuler(CVector3D &v);

	//コピーコンストラクタ
	CBeam(const CBeam& b) {
		//描画用頂点配列を作成
		m_pVertexAry = new CVector3D[m_vertexNum];
		//描画用法線配列を作成
		m_pNormalAry = new CVector3D[m_vertexNum];

		memcpy(m_pVertexAry, b.m_pVertexAry, sizeof(CVector3D) * m_vertexNum);
		memcpy(m_pNormalAry, b.m_pNormalAry, sizeof(CVector3D) * m_vertexNum);

	}
	//代入演算子
	CBeam& setArray(const CBeam& b);

};
