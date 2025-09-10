#pragma once

#include "CMatrix.h"
//OBB�N���X
class COBB {
public:
	CVector3D m_center;		//���S���W
	CVector3D m_axis[3];	//�e���x�N�g��
	float	 m_length[3];	//�e���̒���
	//OBB�ɍs��𔽉f
	void Transeform(CMatrix &mat);
};
//�Փ˔���N���X
class CCollision {
private:
	//separate����ɓ��e����OBB���m���ڐG���Ă��邩���ׂ�
	static bool CompareLength(const COBB &A,const COBB &B,const CVector3D &distVec,const CVector3D &separate);
public:
	//�����m�̏Փ˔���
	static bool CollisionShpere(const CVector3D &centerA,float radiusA,const CVector3D &centerB,float radiusB);
	//OBB���m�̏Փ˔���
	static bool CollisionOBB(const COBB &A,const COBB &B);
	//AABB���m�̏Փ˔���
	static bool CollisionAABB(const CVector3D &minA,const CVector3D &maxA,const CVector3D &minB,const CVector3D &maxB);

	//�_c���O�p�`�|���S����ɂ��邩���ׂ�
	static bool TriangleIntersect(const CVector3D &c,const CVector3D &v0,const  CVector3D &v1, const CVector3D &v2,const CVector3D &n);
	//�����ƎO�p�`�|���S���Ƃ̏Փ˔���
	static bool IntersectTriangleRay(CVector3D *corss,const CVector3D &p1,const  CVector3D &p2,const CVector3D& v0,const  CVector3D &v1,const  CVector3D &v2,float *pt );
	//point����߂�v0��v1����̓_�𒲂ׂ�
	static CVector3D PointOnLineSegmentNearestPoint(const CVector3D &v0,const CVector3D &v1,const CVector3D &point);
	
	static CVector2D PointOnLineSegmentNearestPoint(const CVector2D &v0,const CVector2D &v1,const CVector2D &point);
	//�O�p�|���S���Ƌ��̏Փ˔���
	static bool IntersectTriangleSphere(const CVector3D &v0,const CVector3D &v1,const CVector3D &v2,const CVector3D &center,float radius,CVector3D *cross=0,float *length = 0);
	//�O�p�|���S���ƃJ�v�Z���̏Փ˔���
	static bool IntersectTriangleCapsule(const CVector3D &v0,const CVector3D &v1,const CVector3D &v2,const CVector3D &top,const CVector3D &bottom,float radius,CVector3D *cross=0,float *length = 0);

	//2�����Ԃ̋���
	static float DistanceLine(const CVector3D &s1,const CVector3D &e1,const CVector3D &s2,const CVector3D &e2,CVector3D *cross=NULL,CVector3D *c2=NULL);

	//�_�Ɛ��̋���
	static float DistancePointToLine(const CVector3D &v0,const CVector3D &v1,const CVector3D &p);
	//�_�Ɛ��̋���
	static float DistancePointToLineSq(const CVector3D &v0,const CVector3D &v1,const CVector3D &p);
};