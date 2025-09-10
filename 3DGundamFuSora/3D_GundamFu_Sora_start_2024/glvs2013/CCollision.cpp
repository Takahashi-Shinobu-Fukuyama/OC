#include "CCollision.h"

#define abs fabs

inline float min(float a,float b) {
	return (a<b) ? a:b;
}
void COBB::Transeform(CMatrix &mat) {
	for (int i=0;i<3;i++) m_axis[i] = mat * m_axis[i];
	CVector4D c = CVector4D(m_center.x,m_center.y,m_center.z,1);
	c = c*mat;
	m_center = CVector3D(c.x,c.y,c.z);
}
bool CCollision::CollisionShpere(const CVector3D &centerA, float radiusA, const CVector3D &centerB, float radiusB) {
	CVector3D v = centerB-centerA;
	return (v.LengthSq() >(radiusA + radiusB)*(radiusA + radiusB));
}
bool CCollision::TriangleIntersect(const CVector3D &c,const CVector3D &v0, const CVector3D &v1,const  CVector3D &v2,const CVector3D &n){
	if(CVector3D::Dot(CVector3D::Cross(v1-v0, c-v0),n)<0) return false;
	if(CVector3D::Dot(CVector3D::Cross(v2-v1, c-v1),n)<0) return false; 
	if(CVector3D::Dot(CVector3D::Cross(v0-v2, c-v2),n)<0) return false;
	return true;
}
//線分と三角形ポリゴンとの衝突判定
bool CCollision::IntersectTriangleRay(CVector3D *corss,const CVector3D &p1, const CVector3D &p2,const CVector3D &v0, const CVector3D &v1, const CVector3D &v2,float *pt ) {
	CVector3D e1, e2,normal,pv1,pv2;

	e1 = v1 - v0;
	e2 = v2 - v0;

	//面の法線を求める
	normal = CVector3D::Cross(e1, e2).GetNormalize();

	//始点からポリゴン上のある地点（どこでもいい）へのベクトル
	pv1 = p1-v0;
	//終点からポリゴン上のある地点（どこでもいい）へのベクトル
	pv2 = p2-v0;

	//ポリゴンの法線との内積を求める
	float d1 = CVector3D::Dot(pv1,normal);
	float d2 = CVector3D::Dot(pv2,normal);

	//ポリゴンを貫通していない
	if(d1*d2>0) return false;

	//始点からポリゴンまでの距離と線分の長さの比率を求める
	//接地地点を出すのに使用する
	float t = d1/(d1-d2);
	if( *pt < t ) return false;

	//裏から貫通している場合は衝突していないことにする
	if(t<0) return false;

	//線分と平面の接地地点を求める
	CVector3D c = p1+(p2-p1)*t;

	//接地地点が三角形ポリゴン上か調べる
	if(!TriangleIntersect(c,v0,v1,v2,normal)) return false;
	
	*pt = t;
	*corss = c;

	return true;
}

CVector3D CCollision::PointOnLineSegmentNearestPoint(const CVector3D &v0,const CVector3D &v1,const CVector3D &point){
	CVector3D V = v1-v0;
	float a = V.x*V.x+V.y*V.y+V.z*V.z;
	//線分の始点と終点が同じ場所
	if(a==0) return v0;
	CVector3D VP = point-v0;
	float b = CVector3D::Dot(V,VP);
	float t = b/a;
	//v0よりに遠い場合は近い点をV0に
	if(t<0) return v0;
	//V1よりに遠い場合は近い点をV1に
	if(t>1) return v1;
	//t=<1 t>=0 の場合は線分上に近い点がある 
	return v0 + V*t;
}

CVector2D CCollision::PointOnLineSegmentNearestPoint(const CVector2D &v0,const CVector2D &v1,const CVector2D &point){
	CVector2D V = v1-v0;
	float a = V.x*V.x+V.y*V.y;
	//線分の始点と終点が同じ場所
	if(a==0) return v0;
	CVector2D VP = v0-point;
	float b = CVector2D::Dot(V,VP);
	float t = -(b/a);
	//v0よりに遠い場合は近い点をV0に
	if(t<0) return v0;
	//V1よりに遠い場合は近い点をV1に
	if(t>1) return v1;
	//t=<1 t>=0 の場合は線分上に近い点がある 
	return v0 + V*t;
}
//-----------------------------------------------------------------------------
bool CCollision::IntersectTriangleSphere( const CVector3D &v0,const CVector3D &v1,const CVector3D &v2,const CVector3D &center,float radius ,CVector3D *cross,float *length)
{   
     CVector3D V1(v1-v0);
	 CVector3D V2(v2-v1);
	 CVector3D N(CVector3D::Cross(V1,V2).GetNormalize());
	 
	 CVector3D V = center - v0;
	 //平面と点の距離を求める
	 float Dist = CVector3D::Dot(V,N);
  
	//球の半径より離れている場合は接触無し
	 if(abs(Dist) > radius) return false;

	 //点から平面上に垂直に下ろした地点を求める
	 CVector3D Point =  center - ( N * Dist );
	 
	 
	 //上記の点が三角形ポリゴン内なら接触している
	 if(TriangleIntersect( Point, v0, v1, v2 , N)) {
		if(cross) *cross = Point;
		if(length) *length = Dist;
		return true;
	}

	 //各辺に球がかすっている可能性がある
	 //１辺ごとに球と辺の最短距離を求める

	 //最短距離
	 float l;
	 //最短接触地点
	 CVector3D c;
	 
	 //距離比較用
	 float LengthSq;

	 //辺１(v0→v1)
	 Point = PointOnLineSegmentNearestPoint( v0, v1, center );
	 LengthSq = (center - Point).LengthSq();
	 l  = LengthSq;
	 c = Point;

	 //辺２(v1→v2)
	 Point = PointOnLineSegmentNearestPoint( v1, v2, center );
	 LengthSq = (center - Point).LengthSq();
	 if(l>LengthSq) {
		 l = LengthSq;
		 c = Point;
	 }
	 
	 //辺３(v2→v0)
	 Point = PointOnLineSegmentNearestPoint( v2, v0, center );
	 LengthSq = (center - Point).LengthSq();
	 if(l>LengthSq) {
		 l = LengthSq;
		 c = Point;
	 }

	 //最短距離を確定
	 if(length) *length = sqrt(l);
	 //最短地点を確定
	 if(cross) *cross = c;
	
    return (*length<=radius);
}

bool CCollision::IntersectTriangleCapsule(const CVector3D &v0,const CVector3D &v1,const CVector3D &v2,const CVector3D &top,const CVector3D &bottom,float radius,CVector3D *cross,float *length ){	
	CVector3D V(top-bottom);
	CVector3D  VP;
	float Dist = 1e10,d;

	//ポリゴンの法線を求める
	CVector3D N(CVector3D::Cross(v1 - v0, v2 - v0).GetNormalize());

	//始点からポリゴン上のある地点（どこでもいい）へのベクトル
	CVector3D PV1 = top-v0;
	//終点からポリゴン上のある地点（どこでもいい）へのベクトル
	CVector3D PV2 = bottom-v0;

	//ポリゴンの法線との内積を求める
	float d1 = CVector3D::Dot(PV1,N);
	float d2 = CVector3D::Dot(PV2,N);

	if(d1*d2<0) {
		//貫通している場合は線とポリゴンの判定を行う
		if(IntersectTriangleRay(cross,top+CVector3D(0,radius,0),bottom+CVector3D(0,-radius,0),v0,v1,v2,&Dist)) {
			if(length) {
				//貫通点までの距離を求める
				float lt = (*cross - top).LengthSq();
				float lb = (*cross - bottom).LengthSq();
				if(lt<lb) *length = sqrt(lt);
				else *length = sqrt(lb);
			}
			return true;
		}
	}

	d1=abs(d1);
	d2=abs(d2);
	//平面上の点との最短地点を求める
	CVector3D C1(top-N*d1);
	CVector3D C2(bottom-N*d2);
	//点が平面上にない場合は無効、後の辺との接触で調べる
	if(!TriangleIntersect(C1,v0,v1,v2,N)) d1=1e10;
	if(!TriangleIntersect(C2,v0,v1,v2,N)) d2=1e10;

	//面との距離が近い点の距離を選択
	Dist = (d1<d2) ? d1:d2; 
	
	//各辺との距離を求める
	d = DistanceLine(top,bottom,v0,v1,cross);
	if(Dist > d) Dist = d;
	
	d = DistanceLine(top,bottom,v1,v2,cross);
	if(Dist > d) Dist = d;
	
	d = DistanceLine(top,bottom,v2,v0,cross);
	if(Dist > d) Dist = d;

	if(length) *length = Dist;

    return (Dist<=radius);
}
bool CCollision::CompareLength(const COBB &A,const COBB &B,const CVector3D &distVec,const CVector3D &separate){
	float dist = abs(CVector3D::Dot(distVec,separate));
	float distA = 0;
	for(int i=0;i<3;i++) distA+=abs( CVector3D::Dot(A.m_axis[i],separate)) * A.m_length[i];
	float distB = 0;
	for(int i=0;i<3;i++) distB+=abs( CVector3D::Dot(B.m_axis[i],separate)) * B.m_length[i];
	if(dist > distA + distB) return false;
	return true;
}
bool CCollision::CollisionOBB(const COBB &A,const COBB &B){
	CVector3D distVec = B.m_center - A.m_center;

	for(int i=0;i<3;i++)
		if(!CompareLength(A,B,distVec,A.m_axis[i])) return false;
	
	for(int i=0;i<3;i++)
		if(!CompareLength(A,B,distVec,B.m_axis[i])) return false;
	
	CVector3D separate;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			separate = CVector3D::Cross(A.m_axis[i],B.m_axis[j]);
			if(!CompareLength(A,B,distVec,separate)) return false;
		}
	}

	return true;
}

//AABB同士の衝突判定
bool CCollision::CollisionAABB(const CVector3D &minA,const CVector3D &maxA,const CVector3D &minB,const CVector3D &maxB){
	if( minA.x  < maxB.x && maxA.x > minB.x &&
		minA.y  < maxB.y && maxA.y > minB.y &&
		minA.z  < maxB.z && maxA.z > minB.z) return true;
	return false;
}

//2線分間の距離
float CCollision::DistanceLine(const CVector3D &s1,const CVector3D &e1,const CVector3D &s2,const CVector3D &e2,CVector3D *c,CVector3D *c2){
	//線のベクトルを求める
	CVector3D V1(e1-s1);
	CVector3D V2(e2-s2);
	//2つの線分の外積を求める
	CVector3D N(CVector3D::Cross(V1,V2));
	//2つの線分が平行でない場合
	if (N.LengthSq()) {
		//交差しているか調べる	
		if(CVector3D::Dot(CVector3D::Cross(V1,s1-s2),CVector3D::Cross(V1,s1-e2)) < 0 &&
			CVector3D::Dot(CVector3D::Cross(V2,s2-s1),CVector3D::Cross(V2,s2-e1)) < 0 ) {
				return abs(CVector3D::Dot(N.GetNormalize(), s1-s2));
		}
	}
	//始点と終点から線分との距離を求め、もっとも短い距離を返す
	return sqrt(min(
			min(DistancePointToLineSq(s2, e2, s1),
				DistancePointToLineSq(s2, e2, e1)),
			min(DistancePointToLineSq(s1, e1, s2),
				DistancePointToLineSq(s1, e1, e2))
		));
}
	//点と線の距離
float CCollision::DistancePointToLine(const CVector3D &v0,const CVector3D &v1,const CVector3D &point)
{
	CVector3D p = PointOnLineSegmentNearestPoint(v0,v1,point);
	return (point-p).Length();
}
//点と線の距離
float CCollision::DistancePointToLineSq(const CVector3D &v0,const CVector3D &v1,const CVector3D &point)
{
	CVector3D p = PointOnLineSegmentNearestPoint(v0,v1,point);
	return (point-p).LengthSq();
}