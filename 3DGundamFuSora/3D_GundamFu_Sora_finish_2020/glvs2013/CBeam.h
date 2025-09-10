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
	float sx;	//X�T�C�Y
	float sy;	//Y�T�C�Y
	float sz;	//Z�T�C�Y
	bool m_alive;	//true:�L��  false:����
	CVector3D target_pos; //�ړ��ꏊ
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
	//�����Ƃ̏Փ˔���
	bool IntersectTriangleRay(CVector3D *corss,const CVector3D &p1, const CVector3D &p2,float *pt );
	float* getDiffuse();
	virtual void Start(const CVector3D& pos, const CVector3D& target);
	void Render();
	CVector3D GetEuler(CVector3D &v);

	//�R�s�[�R���X�g���N�^
	CBeam(const CBeam& b) {
		//�`��p���_�z����쐬
		m_pVertexAry = new CVector3D[m_vertexNum];
		//�`��p�@���z����쐬
		m_pNormalAry = new CVector3D[m_vertexNum];

		memcpy(m_pVertexAry, b.m_pVertexAry, sizeof(CVector3D) * m_vertexNum);
		memcpy(m_pNormalAry, b.m_pNormalAry, sizeof(CVector3D) * m_vertexNum);

	}
	//������Z�q
	CBeam& setArray(const CBeam& b);

};
