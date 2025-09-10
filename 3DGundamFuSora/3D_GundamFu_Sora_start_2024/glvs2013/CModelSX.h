/*
	Xファイル（スタティックモデル）のモデルクラス
*/

#pragma once
#include "CXSModelLoader.h"
#include "CModel.h"
#include "CTexture.h"
#include "CShader.h"
#include "CCollision.h"

struct SSModelMaterial{
	CXSColorRGBA faceColor;
	float power;
	CXSColorRGB specularColor;
	CXSColorRGB emissiveColor;
	CTexture	*texture;
	SSModelMaterial() {
		texture = NULL;
	}
	~SSModelMaterial() {
		delete texture;
	}

};
class CSPoly {
	CVector3D *m_vertex;
	int			m_vertexCnt;
	CVector3D *m_normal;
	CXSTexCoord *m_texCoord;
public:
	~CSPoly();
	void create(CXSModelLoader &loader,int materialNo);
	void draw();

};
#define POLY_CUT 5

class CCollTriangle {
public:
	CVector3D	m_normal;
	float	m_dist;
};
struct STriangle{
	int	idxv[3];
	int	idxn[3];
	int	mat;
	STriangle(){
	}
	STriangle(int *v,int *n,int m) {
		set(v,n,m);
	}
	void set(int *v,int *n,int m){
		for(int i=0;i<3;i++) {
			idxv[i] = v[i];
			idxn[i] = n[i];
		}
		mat = m;
	}
	
	
	bool rayTest(CVector3D *c,CVector3D p1,CVector3D p2,CVector3D *vertex,CVector3D *normal,float *t) {
		return CCollision::IntersectTriangleRay(c,p1,p2,vertex[idxv[0]],vertex[idxv[1]],vertex[idxv[2]],t);
	}
	bool sphereTest(CVector3D *c,float *length,CVector3D center,float raduis,CVector3D *vertex) {
		return CCollision::IntersectTriangleSphere(vertex[idxv[0]],vertex[idxv[1]],vertex[idxv[2]],center,raduis,c,length);
	}
	bool cupselTest(CVector3D *c,float *length,CVector3D top,CVector3D bottom,float raduis,CVector3D *vertex) {
		return CCollision::IntersectTriangleCapsule(vertex[idxv[0]],vertex[idxv[1]],vertex[idxv[2]],top,bottom,raduis,c,length);
	}
};
struct STriangleList {
	STriangle *m_tri;
	STriangleList *m_next;
	STriangleList() {
		m_next = NULL;
	}
	STriangleList(STriangle *t) {
		m_tri = t;
		m_next = NULL;
	}

};
class CSCollBase{
	STriangleList	*m_Top;
	STriangleList	*m_Current;
public:
	CSCollBase(){
		m_Top = NULL;
		m_Current = NULL;
	}
	~CSCollBase(){
		STriangleList *t=m_Top;
		while(t) {
			STriangleList *t2 = t->m_next;
			delete t;
			t = t2;
		}
	}
	void add(STriangle *data) {
		STriangleList *t = new STriangleList(data);
		if(!m_Top) m_Top = t;
		if(m_Current) m_Current->m_next=t;
		m_Current = t;
	}
	bool rayTest(CVector3D *c,CVector3D p1,CVector3D p2,CVector3D *vertex,CVector3D *normal) {
		STriangleList *t=m_Top;
		bool result = false;
		float lenge = 1e10;
		while(t) {
			if(t->m_tri->rayTest(c,p1,p2,vertex,normal,&lenge))
				result = true;
			t = t->m_next;
		}
		CVector3D v = p2-p1;
		if(result) *c = p1 + v*lenge;
		return result;
	}
	bool sphereTest(CVector3D *cross,CVector3D *c_normal,float *length,CVector3D center,float radius,CVector3D *vertex,CVector3D *normal) {
		STriangleList *t=m_Top;
		bool result = false;
		CVector3D c;
		float l = 1e10;
		*length = l;
		
		while(t) {
			if(t->m_tri->sphereTest(&c,&l,center,radius,vertex)) {
				
				result = true;
				if(l < *length) {
					*length=l;
					*cross = c;
					*c_normal = normal[t->m_tri->idxn[0]];
				}
			}
			t = t->m_next;
		}
		return result;
	}
	int sphereTest(CCollTriangle *out,CVector3D center,float radius,CVector3D *vertex,CVector3D *normal) {
		STriangleList *t=m_Top;
		int num = 0;
		CVector3D c;
		float l;

		while(t) {
			if(t->m_tri->sphereTest(&c,&l,center,radius,vertex)) {
				
				out[num].m_normal = normal[t->m_tri->idxn[0]];
				out[num].m_dist = l;
				num++;
			}
			t = t->m_next;
		}
		return num;
	}
	int cupselTest(CCollTriangle *out,CVector3D top,CVector3D bottom,float radius,CVector3D *vertex,CVector3D *normal) {
		STriangleList *t=m_Top;
		int num = 0;
		CVector3D c;
		float l;

		while(t) {
			if(t->m_tri->cupselTest(&c,&l,top,bottom,radius,vertex)) {
				out[num].m_normal = normal[t->m_tri->idxn[0]];
				out[num].m_dist = l;
				num++;
			}
			t = t->m_next;
		}
		return num;
	}
};

class CSCollPoly {
	STriangle *m_triData;
	CVector3D *m_vertex;
	CVector3D *m_normal;
	CSCollBase *m_tri;
public:
	CSCollPoly(CXSModelLoader &loader,int cut);
	~CSCollPoly();
	bool rayTest(CVector3D *c,CVector3D s,CVector3D e);
	bool sphereTest(CVector3D *c,CVector3D *c_n,float *length,CVector3D center,float radius);
	int	sphereTest(CCollTriangle *out,CVector3D center,float radius);
	int	cupselTest(CCollTriangle *out,CVector3D top,CVector3D bottom,float radius);
	//void draw();

};
#define CMODEL_MODE_DRAW (1<<0)
#define CMODEL_MODE_COLLISION (1<<1)
class CModelSX : public CModel{
private:
	CSPoly		*m_poly;
	CSCollPoly	*m_coll;
	SSModelMaterial *m_material;
	int			m_materialNum;
	int			m_mode;
	CShader		*m_shader;
	float alpha;
public:
	CModelSX();
	~CModelSX();
	bool Load(const char *path) {
		return Load(path,CMODEL_MODE_DRAW);
	}
	bool Load(const char *filePath,int mode);
	void RenderM(CMatrix &m);
	void Release();
	bool CollisionRay(CVector3D *c,CVector3D s,CVector3D e){
		if(m_coll) {
			CMatrix inv = m_matrix.getInverse();
			CVector4D s2 = CVector4D(s.x,s.y,s.z,1);
			s2 = s2*inv;
			CVector4D e2 = CVector4D(e.x,e.y,e.z,1);
			e2 = e2*inv;
		
			if(m_coll->rayTest(c,CVector3D(s2.x,s2.y,s2.z),CVector3D(e2.x,e2.y,e2.z))) {
				CVector4D c2 = CVector4D(c->x,c->y,c->z,1);
				c2 = c2*m_matrix;
				*c = CVector3D(c2.x,c2.y,c2.z);
				return true;
			}	
		}
		return false;
	}
	
	bool CollisionSphere(CVector3D *c,CVector3D *n,float *length,CVector3D center,float radius){
		float scale = CVector3D (m_matrix.m00,m_matrix.m10,m_matrix.m20).Length();
		radius = radius / scale;
		CVector4D center2 = CVector4D(center.x,center.y,center.z,1);
		center2 = center2*m_matrix.getInverse();
		if(m_coll) {
			if(m_coll->sphereTest(c,n,length,CVector3D(center2.x,center2.y,center2.z),radius)) {
				CVector4D cross = CVector4D(c->x,c->y,c->z,1);
				cross = cross * m_matrix;
				*n = *n * m_matrix;
				n->Normalize();
				*c = CVector3D(cross.x,cross.y,cross.z);
				*length = *length*scale;
				return true;
			}	
		}
		return false;
			
	}
	int CollisionSphere(CCollTriangle *out,CVector3D center,float radius){
		float scale = CVector3D (m_matrix.m00,m_matrix.m10,m_matrix.m20).Length();
		radius = radius / scale;
		CVector4D center2 = CVector4D(center.x,center.y,center.z,1);
		center2 = center2*m_matrix.getInverse();
		if(m_coll) {
			int n = m_coll->sphereTest(out,CVector3D(center2.x,center2.y,center2.z),radius);
			for(int i=0;i<n;i++) {
				out[i].m_normal = out[i].m_normal * m_matrix;
				out[i].m_normal.Normalize();
				out[i].m_dist = out[i].m_dist * scale;
			}
			return n;
		}
		return 0;
			
	}
	int CollisionCupsel(CCollTriangle *out,CVector3D top,CVector3D bottom,float radius){
		float scale = CVector3D (m_matrix.m00,m_matrix.m10,m_matrix.m20).Length();
		radius = radius / scale;
		top = top*m_matrix.getInverse();
		bottom = bottom*m_matrix.getInverse();
		if(m_coll) {
			int n = m_coll->cupselTest(out,top,bottom,radius);
			for(int i=0;i<n;i++) {
				out[i].m_normal = out[i].m_normal * m_matrix;
				out[i].m_normal.Normalize();
				out[i].m_dist = out[i].m_dist * scale;
			}
			return n;
		}
		return 0;
			
	}
	
	
	void setAlhpa(float a){
		alpha = a;
	}
};