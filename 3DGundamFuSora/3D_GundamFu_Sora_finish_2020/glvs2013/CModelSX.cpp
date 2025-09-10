#include "CModelSX.h"
#include "CLight.h"
#include "CCamera.h"

CSPoly::~CSPoly(){
	delete[] m_vertex;
	delete[] m_normal;
	delete[] m_texCoord;
}
void CSPoly::create(CXSModelLoader &loader,int materialNo){
	
	m_vertexCnt = 0;
	for(int i=0;i<loader.m_faceNum;i++) {
		if(loader.m_materialList[i] == materialNo) m_vertexCnt++;
	}
	m_vertexCnt*=3;
	m_vertex = new CVector3D[m_vertexCnt];
	if(m_normal)
		m_normal = new CVector3D[m_vertexCnt];
	if(m_texCoord)
		m_texCoord = new CXSTexCoord[m_vertexCnt];
	CVector3D *v = m_vertex;
	CVector3D *n = m_normal;
	CXSTexCoord *t = m_texCoord;
	for(int i=0;i<loader.m_faceNum;i++) {
		if(loader.m_materialList[i] != materialNo) continue;
		for(int j=0;j<3;j++) {
			v[j].x = loader.m_vertex[loader.m_vertexIndex[i*3+j]].x;
			v[j].y = loader.m_vertex[loader.m_vertexIndex[i*3+j]].y;
			v[j].z = loader.m_vertex[loader.m_vertexIndex[i*3+j]].z;
		}
		v+=3;
		if(n) {
			for(int j=0;j<3;j++) {
				n[j].x = loader.m_normal[loader.m_normalIndex[i*3+j]].x;
				n[j].y = loader.m_normal[loader.m_normalIndex[i*3+j]].y;
				n[j].z = loader.m_normal[loader.m_normalIndex[i*3+j]].z;
			}
			n+=3;
		}
		if(t) {
			for(int j=0;j<3;j++) {
				t[j].u = loader.m_texCoord[loader.m_vertexIndex[i*3+j]].u;
				t[j].v = loader.m_texCoord[loader.m_vertexIndex[i*3+j]].v;
			}
			t+=3;
		}
		
	}

}
void CSPoly::draw(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,m_vertex);
	if(m_normal) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT,0,m_normal);
	}
	if(m_texCoord) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,0,m_texCoord);
	}
	glDrawArrays(GL_TRIANGLES,0,m_vertexCnt);
	glDisableClientState(GL_VERTEX_ARRAY);
	if(m_normal) { 
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	if(m_texCoord) {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
	}
	
}
CSCollPoly::CSCollPoly(CXSModelLoader &loader,int cut) {
	m_vertex = new CVector3D[loader.m_vertexNum];
	memcpy(m_vertex,loader.m_vertex,sizeof(CVector3D)*loader.m_vertexNum);
	m_normal = new CVector3D[loader.m_normalNum];
	memcpy(m_normal,loader.m_normal,sizeof(CVector3D)*loader.m_normalNum);
	m_triData = new STriangle[loader.m_faceNum];
	for(int i=0;i<loader.m_faceNum;i++) {
		m_triData[i].set(&loader.m_vertexIndex[i*3],&loader.m_normalIndex[i*3],loader.m_materialList[i]);
	}
	m_tri = new CSCollBase;
	for(int i=0;i<loader.m_faceNum;i++) {
		m_tri->add(&m_triData[i]);
	}
}
CSCollPoly::~CSCollPoly() {
	delete[] m_vertex;
	delete[] m_normal;
	delete m_tri;
	delete[] m_triData;
}

bool CSCollPoly::rayTest(CVector3D *c,CVector3D s,CVector3D e){
	return m_tri->rayTest(c,s,e,m_vertex,m_normal);
		
}
bool CSCollPoly::sphereTest(CVector3D *c,CVector3D *n,float *length,CVector3D center,float radius){
	return m_tri->sphereTest(c,n,length,center,radius,m_vertex,m_normal);
		
}
int CSCollPoly::sphereTest(CCollTriangle *out,CVector3D center,float radius){
	return m_tri->sphereTest(out,center,radius,m_vertex,m_normal);
		
}
int CSCollPoly::cupselTest(CCollTriangle *out,CVector3D top,CVector3D bottom,float radius){
	return m_tri->cupselTest(out,top,bottom,radius,m_vertex,m_normal);
		
}
CModelSX::CModelSX() : m_poly(NULL),m_coll(NULL),m_material(NULL),m_shader(NULL),alpha(1.0f){
}

CModelSX::~CModelSX() {
}
void CModelSX::Release() {
	delete[] m_poly;
	delete m_coll;
	delete[] m_material;
	delete m_shader;

}
bool CModelSX::Load(const char *filePath,int mode){
	CXSModelLoader loader(filePath);
	m_material = new SSModelMaterial[loader.m_materialNum];
	m_materialNum = loader.m_materialNum;
	if(mode & CMODEL_MODE_DRAW) {
		m_poly = new CSPoly[m_materialNum];
		for(int i=0;i<m_materialNum;i++) {
			SSModelMaterial *m = &m_material[i];
			m->faceColor = loader.m_material[i].faceColor;
			m->power = loader.m_material[i].power;
			m->emissiveColor = loader.m_material[i].emissiveColor;
			m->specularColor = loader.m_material[i].specularColor;
			if(loader.m_material[i].filePath[0]) {
				m->texture = new CTexture();
				m->texture->Load(loader.m_material[i].filePath);
			}
			m_poly[i].create(loader,i);
		}
		m_shader = new CShader("mesh.vert","mesh.flag");
	}
	if(mode & CMODEL_MODE_COLLISION) {
		m_coll = new CSCollPoly(loader,0);
	}
	m_mode = mode;
	return true;
}
void CModelSX::RenderM(CMatrix &m){
	if(!(m_mode & CMODEL_MODE_DRAW)) return;
	m_shader->enable();

	/*
	  ライト設定
	*/
	CVector3D vec=CLight::getLight(0)->getDir();
	vec*=-1;
	int lightId = glGetUniformLocation(m_shader->getProgram(),"lightVec");  //ライトの向きを設定
	glUniform3fv(lightId,1,(float*)&vec);


	glUniform3fv(glGetUniformLocation(m_shader->getProgram(),"lightAmbientColor"),1,(float*)&(CLight::getLight(0)->getAmbientColor()));
	glUniform3fv(glGetUniformLocation(m_shader->getProgram(),"lightDiffuseColor"),1,(float*)&(CLight::getLight(0)->getDiffuseColor()));
		
	/*
	  視線設定
	*/
	vec=CCamera::getCurrent()->getDir();
	vec*=-1;
	int eyeId = glGetUniformLocation(m_shader->getProgram(),"eyeVec");  //ライトの向きを設定
	glUniform3fv(eyeId,1,(float*)&vec);
		
	/*
	  ワールドトランスフォーム
	*/
	int worldId = glGetUniformLocation(m_shader->getProgram(),"WorldMatrix");  //ライトの向きを設定
	glUniformMatrix4fv(worldId,1,false,m_matrix.f);

	glPushMatrix();
	glMultMatrixf(m_matrix.f);

	for(int i=0;i<m_materialNum;i++) {
		SSModelMaterial *m = &m_material[i];
		int AmbientId = glGetUniformLocation(m_shader->getProgram(),"Ambient");  //ライトの向きを設定
		CXSColorRGBA color;
		color = m->faceColor;
		color.a = color.a*alpha;
		glUniform4fv(AmbientId,1,(GLfloat*)&color);
		
		int DiffuseId = glGetUniformLocation(m_shader->getProgram(),"Diffuse");  //ライトの向きを設定
		glUniform4fv(DiffuseId,1,(GLfloat*)&m->faceColor);
		
		int PowId = glGetUniformLocation(m_shader->getProgram(),"Pow");  //ライトの向きを設定
		glUniform1f(PowId,m->power);
				
		int SpecularId = glGetUniformLocation(m_shader->getProgram(),"Specular");  //ライトの向きを設定
		glUniform3fv(SpecularId,1,(GLfloat*)&m->specularColor);

		
		int EmissiveId = glGetUniformLocation(m_shader->getProgram(),"Emissive");  //ライトの向きを設定
		glUniform3fv(EmissiveId,1,(GLfloat*)&m->emissiveColor);
		if(m->texture) {
			m->texture->MapTexture();
		}
		GLint samplerId = glGetUniformLocation(m_shader->getProgram(), "sampler");
		glUniform1i(samplerId, 0);//GL_TEXTURE1を適用

		glUniform1i(glGetUniformLocation(m_shader->getProgram(), "sampler7"), 7);//GL_TEXTURE7を適用
	  

		m_poly[i].draw();
		if(m->texture) {
			m->texture->UnmapTexture();
		}
	}
	m_shader->disable();
	glPopMatrix();

}