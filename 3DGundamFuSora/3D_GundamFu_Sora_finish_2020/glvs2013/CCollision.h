#pragma once

#include "CMatrix.h"
//OBBクラス
class COBB {
public:
	CVector3D m_center;		//中心座標
	CVector3D m_axis[3];	//各軸ベクトル
	float	 m_length[3];	//各軸の長さ
	//OBBに行列を反映
	void Transeform(CMatrix &mat);
};
//衝突判定クラス
class CCollision {
private:
	//separate軸上に投影したOBB同士が接触しているか調べる
	static bool CompareLength(const COBB &A,const COBB &B,const CVector3D &distVec,const CVector3D &separate);
public:
	//球同士の衝突判定
	static bool CollisionShpere(const CVector3D &centerA,float radiusA,const CVector3D &centerB,float radiusB);
	//OBB同士の衝突判定
	static bool CollisionOBB(const COBB &A,const COBB &B);
	//AABB同士の衝突判定
	static bool CollisionAABB(const CVector3D &minA,const CVector3D &maxA,const CVector3D &minB,const CVector3D &maxB);

	//点cが三角形ポリゴン上にあるか調べる
	static bool TriangleIntersect(const CVector3D &c,const CVector3D &v0,const  CVector3D &v1, const CVector3D &v2,const CVector3D &n);
	//線分と三角形ポリゴンとの衝突判定
	static bool IntersectTriangleRay(CVector3D *corss,const CVector3D &p1,const  CVector3D &p2,const CVector3D& v0,const  CVector3D &v1,const  CVector3D &v2,float *pt );
	//pointから近いv0→v1線上の点を調べる
	static CVector3D PointOnLineSegmentNearestPoint(const CVector3D &v0,const CVector3D &v1,const CVector3D &point);
	
	static CVector2D PointOnLineSegmentNearestPoint(const CVector2D &v0,const CVector2D &v1,const CVector2D &point);
	//三角ポリゴンと球の衝突判定
	static bool IntersectTriangleSphere(const CVector3D &v0,const CVector3D &v1,const CVector3D &v2,const CVector3D &center,float radius,CVector3D *cross=0,float *length = 0);
	//三角ポリゴンとカプセルの衝突判定
	static bool IntersectTriangleCapsule(const CVector3D &v0,const CVector3D &v1,const CVector3D &v2,const CVector3D &top,const CVector3D &bottom,float radius,CVector3D *cross=0,float *length = 0);

	//2線分間の距離
	static float DistanceLine(const CVector3D &s1,const CVector3D &e1,const CVector3D &s2,const CVector3D &e2,CVector3D *cross=NULL,CVector3D *c2=NULL);

	//点と線の距離
	static float DistancePointToLine(const CVector3D &v0,const CVector3D &v1,const CVector3D &p);
	//点と線の距離
	static float DistancePointToLineSq(const CVector3D &v0,const CVector3D &v1,const CVector3D &p);
};