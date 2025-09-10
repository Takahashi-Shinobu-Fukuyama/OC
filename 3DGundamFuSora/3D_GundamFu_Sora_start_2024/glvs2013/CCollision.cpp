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
//�����ƎO�p�`�|���S���Ƃ̏Փ˔���
bool CCollision::IntersectTriangleRay(CVector3D *corss,const CVector3D &p1, const CVector3D &p2,const CVector3D &v0, const CVector3D &v1, const CVector3D &v2,float *pt ) {
	CVector3D e1, e2,normal,pv1,pv2;

	e1 = v1 - v0;
	e2 = v2 - v0;

	//�ʂ̖@�������߂�
	normal = CVector3D::Cross(e1, e2).GetNormalize();

	//�n�_����|���S����̂���n�_�i�ǂ��ł������j�ւ̃x�N�g��
	pv1 = p1-v0;
	//�I�_����|���S����̂���n�_�i�ǂ��ł������j�ւ̃x�N�g��
	pv2 = p2-v0;

	//�|���S���̖@���Ƃ̓��ς����߂�
	float d1 = CVector3D::Dot(pv1,normal);
	float d2 = CVector3D::Dot(pv2,normal);

	//�|���S�����ђʂ��Ă��Ȃ�
	if(d1*d2>0) return false;

	//�n�_����|���S���܂ł̋����Ɛ����̒����̔䗦�����߂�
	//�ڒn�n�_���o���̂Ɏg�p����
	float t = d1/(d1-d2);
	if( *pt < t ) return false;

	//������ђʂ��Ă���ꍇ�͏Փ˂��Ă��Ȃ����Ƃɂ���
	if(t<0) return false;

	//�����ƕ��ʂ̐ڒn�n�_�����߂�
	CVector3D c = p1+(p2-p1)*t;

	//�ڒn�n�_���O�p�`�|���S���ォ���ׂ�
	if(!TriangleIntersect(c,v0,v1,v2,normal)) return false;
	
	*pt = t;
	*corss = c;

	return true;
}

CVector3D CCollision::PointOnLineSegmentNearestPoint(const CVector3D &v0,const CVector3D &v1,const CVector3D &point){
	CVector3D V = v1-v0;
	float a = V.x*V.x+V.y*V.y+V.z*V.z;
	//�����̎n�_�ƏI�_�������ꏊ
	if(a==0) return v0;
	CVector3D VP = point-v0;
	float b = CVector3D::Dot(V,VP);
	float t = b/a;
	//v0���ɉ����ꍇ�͋߂��_��V0��
	if(t<0) return v0;
	//V1���ɉ����ꍇ�͋߂��_��V1��
	if(t>1) return v1;
	//t=<1 t>=0 �̏ꍇ�͐�����ɋ߂��_������ 
	return v0 + V*t;
}

CVector2D CCollision::PointOnLineSegmentNearestPoint(const CVector2D &v0,const CVector2D &v1,const CVector2D &point){
	CVector2D V = v1-v0;
	float a = V.x*V.x+V.y*V.y;
	//�����̎n�_�ƏI�_�������ꏊ
	if(a==0) return v0;
	CVector2D VP = v0-point;
	float b = CVector2D::Dot(V,VP);
	float t = -(b/a);
	//v0���ɉ����ꍇ�͋߂��_��V0��
	if(t<0) return v0;
	//V1���ɉ����ꍇ�͋߂��_��V1��
	if(t>1) return v1;
	//t=<1 t>=0 �̏ꍇ�͐�����ɋ߂��_������ 
	return v0 + V*t;
}
//-----------------------------------------------------------------------------
bool CCollision::IntersectTriangleSphere( const CVector3D &v0,const CVector3D &v1,const CVector3D &v2,const CVector3D &center,float radius ,CVector3D *cross,float *length)
{   
     CVector3D V1(v1-v0);
	 CVector3D V2(v2-v1);
	 CVector3D N(CVector3D::Cross(V1,V2).GetNormalize());
	 
	 CVector3D V = center - v0;
	 //���ʂƓ_�̋��������߂�
	 float Dist = CVector3D::Dot(V,N);
  
	//���̔��a��藣��Ă���ꍇ�͐ڐG����
	 if(abs(Dist) > radius) return false;

	 //�_���畽�ʏ�ɐ����ɉ��낵���n�_�����߂�
	 CVector3D Point =  center - ( N * Dist );
	 
	 
	 //��L�̓_���O�p�`�|���S�����Ȃ�ڐG���Ă���
	 if(TriangleIntersect( Point, v0, v1, v2 , N)) {
		if(cross) *cross = Point;
		if(length) *length = Dist;
		return true;
	}

	 //�e�ӂɋ����������Ă���\��������
	 //�P�ӂ��Ƃɋ��ƕӂ̍ŒZ���������߂�

	 //�ŒZ����
	 float l;
	 //�ŒZ�ڐG�n�_
	 CVector3D c;
	 
	 //������r�p
	 float LengthSq;

	 //�ӂP(v0��v1)
	 Point = PointOnLineSegmentNearestPoint( v0, v1, center );
	 LengthSq = (center - Point).LengthSq();
	 l  = LengthSq;
	 c = Point;

	 //�ӂQ(v1��v2)
	 Point = PointOnLineSegmentNearestPoint( v1, v2, center );
	 LengthSq = (center - Point).LengthSq();
	 if(l>LengthSq) {
		 l = LengthSq;
		 c = Point;
	 }
	 
	 //�ӂR(v2��v0)
	 Point = PointOnLineSegmentNearestPoint( v2, v0, center );
	 LengthSq = (center - Point).LengthSq();
	 if(l>LengthSq) {
		 l = LengthSq;
		 c = Point;
	 }

	 //�ŒZ�������m��
	 if(length) *length = sqrt(l);
	 //�ŒZ�n�_���m��
	 if(cross) *cross = c;
	
    return (*length<=radius);
}

bool CCollision::IntersectTriangleCapsule(const CVector3D &v0,const CVector3D &v1,const CVector3D &v2,const CVector3D &top,const CVector3D &bottom,float radius,CVector3D *cross,float *length ){	
	CVector3D V(top-bottom);
	CVector3D  VP;
	float Dist = 1e10,d;

	//�|���S���̖@�������߂�
	CVector3D N(CVector3D::Cross(v1 - v0, v2 - v0).GetNormalize());

	//�n�_����|���S����̂���n�_�i�ǂ��ł������j�ւ̃x�N�g��
	CVector3D PV1 = top-v0;
	//�I�_����|���S����̂���n�_�i�ǂ��ł������j�ւ̃x�N�g��
	CVector3D PV2 = bottom-v0;

	//�|���S���̖@���Ƃ̓��ς����߂�
	float d1 = CVector3D::Dot(PV1,N);
	float d2 = CVector3D::Dot(PV2,N);

	if(d1*d2<0) {
		//�ђʂ��Ă���ꍇ�͐��ƃ|���S���̔�����s��
		if(IntersectTriangleRay(cross,top+CVector3D(0,radius,0),bottom+CVector3D(0,-radius,0),v0,v1,v2,&Dist)) {
			if(length) {
				//�ђʓ_�܂ł̋��������߂�
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
	//���ʏ�̓_�Ƃ̍ŒZ�n�_�����߂�
	CVector3D C1(top-N*d1);
	CVector3D C2(bottom-N*d2);
	//�_�����ʏ�ɂȂ��ꍇ�͖����A��̕ӂƂ̐ڐG�Œ��ׂ�
	if(!TriangleIntersect(C1,v0,v1,v2,N)) d1=1e10;
	if(!TriangleIntersect(C2,v0,v1,v2,N)) d2=1e10;

	//�ʂƂ̋������߂��_�̋�����I��
	Dist = (d1<d2) ? d1:d2; 
	
	//�e�ӂƂ̋��������߂�
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

//AABB���m�̏Փ˔���
bool CCollision::CollisionAABB(const CVector3D &minA,const CVector3D &maxA,const CVector3D &minB,const CVector3D &maxB){
	if( minA.x  < maxB.x && maxA.x > minB.x &&
		minA.y  < maxB.y && maxA.y > minB.y &&
		minA.z  < maxB.z && maxA.z > minB.z) return true;
	return false;
}

//2�����Ԃ̋���
float CCollision::DistanceLine(const CVector3D &s1,const CVector3D &e1,const CVector3D &s2,const CVector3D &e2,CVector3D *c,CVector3D *c2){
	//���̃x�N�g�������߂�
	CVector3D V1(e1-s1);
	CVector3D V2(e2-s2);
	//2�̐����̊O�ς����߂�
	CVector3D N(CVector3D::Cross(V1,V2));
	//2�̐��������s�łȂ��ꍇ
	if (N.LengthSq()) {
		//�������Ă��邩���ׂ�	
		if(CVector3D::Dot(CVector3D::Cross(V1,s1-s2),CVector3D::Cross(V1,s1-e2)) < 0 &&
			CVector3D::Dot(CVector3D::Cross(V2,s2-s1),CVector3D::Cross(V2,s2-e1)) < 0 ) {
				return abs(CVector3D::Dot(N.GetNormalize(), s1-s2));
		}
	}
	//�n�_�ƏI�_��������Ƃ̋��������߁A�����Ƃ��Z��������Ԃ�
	return sqrt(min(
			min(DistancePointToLineSq(s2, e2, s1),
				DistancePointToLineSq(s2, e2, e1)),
			min(DistancePointToLineSq(s1, e1, s2),
				DistancePointToLineSq(s1, e1, e2))
		));
}
	//�_�Ɛ��̋���
float CCollision::DistancePointToLine(const CVector3D &v0,const CVector3D &v1,const CVector3D &point)
{
	CVector3D p = PointOnLineSegmentNearestPoint(v0,v1,point);
	return (point-p).Length();
}
//�_�Ɛ��̋���
float CCollision::DistancePointToLineSq(const CVector3D &v0,const CVector3D &v1,const CVector3D &point)
{
	CVector3D p = PointOnLineSegmentNearestPoint(v0,v1,point);
	return (point-p).LengthSq();
}