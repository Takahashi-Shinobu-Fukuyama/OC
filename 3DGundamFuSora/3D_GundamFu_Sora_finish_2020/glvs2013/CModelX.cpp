#include "CModelX.h"
#include "CLight.h"
#include "CCamera.h"
#include "windows.h"

void CFrameBone::calcBoneMatrix(CMatrix *out){
	boneMatrix = sMatrix * matrixOffset;

	out[getNo()] = boneMatrix;
}
void CFrameBone::calcMatrix(){
	if(getParent()) sMatrix = ((CFrameBone*)getParent())->getSMatrix() * getMatrix();
	else sMatrix = getMatrix();

	if(getChild()) ((CFrameBone*)getChild())->calcMatrix();
	if(getNext()) ((CFrameBone*)getNext())->calcMatrix();
	
}
CMaterial::CMaterial( CXMaterial &m){
	faceColor = m.faceColor;
	power = m.power;
	specularColor = m.specularColor;
	emissiveColor = m.emissiveColor;
	if(strlen(m.filename)) {
		texture = new CTexture();
		if(!texture->Load(m.filename)) {
			delete texture;
			texture = NULL;
		}
	} else {
		texture = NULL;
	}
}
CMaterial::~CMaterial() {
	SAFE_DELETE(texture);
}
void CMaterial::set(CShader *shader){
	
	int AmbientId = glGetUniformLocation(shader->getProgram(),"Ambient");  //カラー設定
	glUniform4fv(AmbientId,1,(GLfloat*)&faceColor);
	
	int DiffuseId = glGetUniformLocation(shader->getProgram(),"Diffuse");  //カラー設定
	glUniform4fv(DiffuseId,1,(GLfloat*)&faceColor);
	
	int PowId = glGetUniformLocation(shader->getProgram(),"Pow");  //ライトの向きを設定
	glUniform1f(PowId,power);
			
	int SpecularId = glGetUniformLocation(shader->getProgram(),"Specular");  //カラー設定
	glUniform3fv(SpecularId,1,(GLfloat*)&specularColor);

	
	int EmissiveId = glGetUniformLocation(shader->getProgram(),"Emissive");  //カラー設定
	glUniform3fv(EmissiveId,1,(GLfloat*)&emissiveColor);
	GLint samplerId = glGetUniformLocation(shader->getProgram(), "sampler");
	GLint textureFlg = glGetUniformLocation(shader->getProgram(), "textureFlg");
	if(texture) {
		texture->MapTexture();
		glUniform1i(samplerId, 0);//GL_TEXTURE1を適用
		glUniform1i(textureFlg, 0);//GL_TEXTURE1を適用
	}
	else
	{
		glUniform1i(textureFlg, -1);//GL_TEXTURE1を適用
	}
	
}
void CMaterial::reset(){
	if(texture) {
		texture->UnmapTexture();
	}
}
CSkinWeights::CSkinWeights(CXModelLoader &loader, CXMesh &mesh){
	weight = new SSkinWeight[mesh.nVertices];
	memset(weight,0,sizeof(SSkinWeight)*mesh.nVertices);
	std::vector<CXSkinWeights*> *skin = &mesh.skinweights;
	std::vector<CXSkinWeights*>::iterator it = skin->begin();
	while(it!=skin->end()) {
		CXSkinWeights* s = *it;
		int bonenum = loader.getFrameNum(s->transformNodeName);
		for(int i=0;i<s->nWeights;i++) {
			int j;
			for(j=0;j<4;j++) {
				if(weight[s->vertexIndices[i]].weight.f[j] == 0.0f) break;
			}
			weight[s->vertexIndices[i]].bone.f[j] = bonenum;
			if(j<=3) weight[s->vertexIndices[i]].weight.f[j] = s->weights[i];

		}
		it++;

	}

}
CSkinWeights::~CSkinWeights() {
	SAFE_DELETE(weight);
}
CPoly::CPoly( CXModelLoader &loader,  CXMesh &mesh,CSkinWeights *skinWeights,int matNo){
	m_pVertex = NULL;

	
	material = loader.getMaterialNum(mesh.meshMaterialList->material[matNo]);
	faceCnt = 0;
	for(int i=0;i<mesh.meshMaterialList->nFaceIndexes;i++) {
		if(mesh.meshMaterialList->faceIndexes[i]==matNo) faceCnt++;
	}
	vertexCnt = faceCnt*3;
	
	bNormal = (mesh.meshNormal) ? true : false;

	
	bTexCode = (mesh.meshTextureCoords) ? true:false;

	bWeight = (skinWeights) ? true:false;

	m_pVertex = new MY_VERTEX[vertexCnt];
	for(int i=0,j=0;i<mesh.meshMaterialList->nFaceIndexes;i++) {
		if(mesh.meshMaterialList->faceIndexes[i]==matNo) {
			for(int k=0;k<3;k++,j++) {
				m_pVertex[j].vPos.x = mesh.vertices[mesh.faces[i].idx[k]].x;
				m_pVertex[j].vPos.y = mesh.vertices[mesh.faces[i].idx[k]].y;
				m_pVertex[j].vPos.z = mesh.vertices[mesh.faces[i].idx[k]].z;
				if(bNormal) {
					m_pVertex[j].vNorm.x = mesh.meshNormal->normals[mesh.meshNormal->faceNormals[i].idx[k]].x;
					m_pVertex[j].vNorm.y = mesh.meshNormal->normals[mesh.meshNormal->faceNormals[i].idx[k]].y;
					m_pVertex[j].vNorm.z = mesh.meshNormal->normals[mesh.meshNormal->faceNormals[i].idx[k]].z;
				}
				if(bTexCode) {
					m_pVertex[j].vTex.x = mesh.meshTextureCoords->textureCoords[mesh.faces[i].idx[k]].u;
					m_pVertex[j].vTex.y = mesh.meshTextureCoords->textureCoords[mesh.faces[i].idx[k]].v;
				}
				if(bWeight) {
					memcpy(m_pVertex[j].bBoneIndex,&skinWeights->weight[mesh.faces[i].idx[k]].bone,sizeof(float)*4);
					memcpy(m_pVertex[j].bBoneWeight,&skinWeights->weight[mesh.faces[i].idx[k]].weight,sizeof(float)*4);
				}
			}
		}
	}
//	printWeight();
	glGenBuffers(1,&buffer);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(MY_VERTEX)*vertexCnt,m_pVertex,GL_STATIC_DRAW);
	delete[] m_pVertex;
	m_pVertex = NULL;
	glBindBuffer(GL_ARRAY_BUFFER,0);


	
}
CPoly::~CPoly() {
	glDeleteBuffers(1,&buffer);
	SAFE_DELETE_ARRAY(m_pVertex);

}
void CPoly::draw(CShader *shader) {
	int idx = 0;
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(MY_VERTEX),(void*)idx);
	idx+=sizeof(SVector3D);
	if(bNormal) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT,sizeof(MY_VERTEX),(void*)idx);
	}
	idx+=sizeof(SVector3D);
	if(bTexCode) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,sizeof(MY_VERTEX),(void*)idx);
	}
	idx+=sizeof(SVector2D);
	if(shader && bWeight) {
		int  weightLoc = glGetAttribLocation(shader->getProgram(), "weights");
		glEnableVertexAttribArray(weightLoc);  
		glVertexAttribPointer(weightLoc, 4, GL_FLOAT, GL_TRUE, sizeof(MY_VERTEX),(void*)idx);
		idx+=sizeof(float)*4;
		int  indexLoc = glGetAttribLocation(shader->getProgram(), "indices");
		glEnableVertexAttribArray(indexLoc);  
		glVertexAttribPointer(indexLoc, 4, GL_FLOAT, GL_FALSE, sizeof(MY_VERTEX),(void*)idx);
	}
	glDrawArrays(GL_TRIANGLES,0,vertexCnt);
	glDisableClientState(GL_VERTEX_ARRAY);
	if(bNormal) { 
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	if(bTexCode) {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

CAnimation::CAnimation() {
}
CAnimation::CAnimation(std::vector<CXAnimationSet*> &a){	
	for(unsigned int i=0;i<a.size();i++) {
		animation.push_back(new CXAnimationSet(*a[i]));
	}
	jam=-1;
	pBoneRoot = NULL;
}
CAnimation::~CAnimation(){
	for(std::vector<CXAnimationSet*>::iterator it = animation.begin();it!=animation.end();it++) {
		SAFE_DELETE(*it);
	}
	animation.clear();
}
CMatrix CAnimation::calcMatrix(CXAnimationKey &key,float t){
	CMatrix m[2],result;
	int i;
	for(i=0;i<key.nKeys;i++) {
		if(key.keys[i].time>=(int)t) break;
	}
//	if(i==0) return key.keys[0].matrix;

	m[0] = key.keys[i].matrix;
	m[1] = key.keys[i+1].matrix;
	float r = (float)(key.keys[i].time-t) / (key.keys[i+1].time - key.keys[i].time);

	result = m[0] * r + m[1] * (1.0f-r);
	return result;

}
void CAnimation::changeAnimation(int j,bool l,bool check){
	if(check) {
		if( jam == j) return;
	}
	loop = l;
	jam = j;
	time = 0;
	speed =1.0f;
	endTime = getEndTime(jam);
}
void CAnimation::updateMatrix(){
	if(jam==-1 || pBoneRoot==NULL) return;
	CXAnimationSet* a = animation[jam];
	for(unsigned int i=0;i<a->animations.size();i++){
		CFrameBone *b = (CFrameBone*)CXFrame::getFrameByName(a->animations[i]->boneName,pBoneRoot);
		if(b) b->setMatrix(calcMatrix(*a->animations[i]->animationkey,time));

	}
	
//	CXFrame::printMatrix(pBoneRoot);
}
int CAnimation::getEndTime(int j){
	if(j>=(int)animation.size()) return -1;
	CXAnimationSet* a = animation[j];
	int e = 0;
	for(unsigned int i=0;i<a->animations.size();i++){
		int m = a->animations[i]->animationkey->keys[a->animations[i]->animationkey->nKeys-1].time;
		if(e < m) e = m;
	}
	return e;
}
void CAnimation::upDate(){
	time += speed;
	if(time>=endTime) {
		if(loop) {
			time = 0;
		} else {
			time = endTime;
		}
	}
}

CMesh::CMesh(CXModelLoader &loader,  CXMesh &mesh,CSkinWeights *bone){
	matrix = mesh.getMatrix();
	for(int i=0;i<mesh.meshMaterialList->nMaterials;i++) {
		polyList.push_back(new CPoly(loader,mesh,bone,i));
	}
	if(bone) {
		for(unsigned int i=0;i<mesh.skinweights.size();i++) {
			boneList.push_back(new CBoneOffset(*mesh.skinweights[i]));
		}
	}

}
CMesh::~CMesh() {
	
	for(std::vector<CPoly*>::iterator it = polyList.begin();it!=polyList.end();it++) {
		SAFE_DELETE(*it);
	}
	polyList.clear();
	
	for(std::vector<CBoneOffset*>::iterator it = boneList.begin();it!=boneList.end();it++) {
		SAFE_DELETE(*it);
	}
	boneList.clear();

}
void CMesh::draw(std::vector<CMaterial*> materialList,CShader *shader,CFrameBone *pFrameRoot,CMatrix *boneMatrix,int boneNum) {
//	glPushMatrix();
//	glMultMatrixf(matrix.f);
	if(boneList.size()>0) {
		for(unsigned int i=0;i<boneList.size();i++) {
			CFrameBone *b = (CFrameBone*)CXFrame::getFrameByName(boneList[i]->transformNodeName,pFrameRoot);
			b->seMatrixOffset(boneList[i]->matrixOffset);
			b->calcBoneMatrix(boneMatrix);
		}
		if(shader) {
			int MatrixLocation = glGetUniformLocation(shader->getProgram(), "Transforms");
			glUniformMatrix4fv(MatrixLocation, boneNum, GL_FALSE, boneMatrix[0].f);
		}
	}
	//pFrameRoot->calcBoneMatrix();
	CMatrix mat;
	glGetFloatv(GL_MODELVIEW_MATRIX,mat.f);
	std::vector<CPoly*>::iterator it;
	for(it=polyList.begin();it!=polyList.end();it++) {
		if((*it)->material!=-1) {
			materialList[(*it)->material]->set(shader);
		}
		(*it)->draw(shader);
		if((*it)->material!=-1) {
			materialList[(*it)->material]->reset();
		}

	}
//	glPopMatrix();
}
CModelX::CModelX() {

}

CModelX::~CModelX(){
}
void CModelX::Release(){
	for(std::vector<CMesh*>::iterator it = meshList.begin();it!=meshList.end();it++) {
		SAFE_DELETE(*it);
	}
	meshList.clear();
	for(std::vector<CMaterial*>::iterator it = materialList.begin();it!=materialList.end();it++) {
		SAFE_DELETE(*it);
	}
	materialList.clear();
	if(pFrameRoot) pFrameRoot->release();
	pFrameRoot=NULL;
	SAFE_DELETE(boneMatrix);
	SAFE_DELETE(animation);
	SAFE_DELETE(shader);
}
void CModelX::RenderM(CMatrix &m) {
	
	if(shader) {
		shader->enable();

		/*
		  ライト設定
		*/
		CVector3D vec=CLight::getLight(0)->getDir();
		vec*=-1;
		int lightId = glGetUniformLocation(shader->getProgram(),"lightVec");  //ライトの向きを設定
		glUniform3fv(lightId,1,(float*)&vec);
		glUniform3fv(glGetUniformLocation(shader->getProgram(),"lightAmbientColor"),1,(float*)&(CLight::getLight(0)->getAmbientColor()));
		glUniform3fv(glGetUniformLocation(shader->getProgram(),"lightDiffuseColor"),1,(float*)&(CLight::getLight(0)->getDiffuseColor()));
		
	
		/*
		  視線設定
		*/
		vec=CCamera::getCurrent()->getDir();
		vec*=-1;
		int eyeId = glGetUniformLocation(shader->getProgram(),"eyeVec");  //ライトの向きを設定
		glUniform3fv(eyeId,1,(float*)&vec);

		
		/*
		  ワールドトランスフォーム
		*/
		int worldId = glGetUniformLocation(shader->getProgram(),"WorldMatrix");
		glUniformMatrix4fv(worldId,1,FALSE,m_matrix.f);
	}


	glPushMatrix();
	glMultMatrixf(m.f);
	//pFrameRoot->setMatrix(matrix);
	
	std::vector<CMesh*>::iterator it;
	for(it=meshList.begin();it!=meshList.end();it++) {
		(*it)->draw(materialList,shader,pFrameRoot,boneMatrix,boneNum);
	}
	glPopMatrix();
	if(shader) shader->disable();

}


void CModelX::createFrame(CXFrame *xf,CFrameBone *f,int *num){
	(*num)++;
	printf("%s %s No%d\n",f->getToken(),f->getName(),f->getNo());
	if(xf->getChild()) {
		CFrameBone *c=new CFrameBone();
		*c = *xf->getChild();
		if(!f->getChild()) f->setChild(c);
		c->setParent(f);
		createFrame(xf->getChild(),c,num);
	}
	if(xf->getNext()) {
		CFrameBone *c=new CFrameBone();
		*c = *xf->getNext();
		f->setNext(c);
		c->setPrev(f);
		c->setParent(f->getParent());
		createFrame(xf->getNext(),c,num);
	}
}
bool CModelX::Load(CXModelLoader &loader){
	boneNum = 0;
//	printf("f\n");
//	CXFrame::printFrame(pFrameRoot,0);

	if(0<loader.animationset.size()) {
		
		pFrameRoot = new CFrameBone();
		*pFrameRoot = *loader.pRoot;
		createFrame(loader.pRoot,pFrameRoot,&boneNum);
		boneMatrix = new CMatrix[boneNum];

		animation = new CAnimation(loader.animationset);
		animation->attachFrame(pFrameRoot);
//		CXFrame::printMatrix(pFrameRoot);
		
		

		//アニメーション用
		shader = new CShader("skinmesh.vert","skinmesh.flag");
	} else {
		animation = NULL;
		//アニメーション無し用
		shader = new CShader("mesh.vert", "mesh.flag");
	}

	for(unsigned int i=0;i<loader.materials.size();i++)
		materialList.push_back(new CMaterial(*loader.materials[i]));
	
	for(unsigned int i=0;i<loader.mesh.size();i++){
		CXMesh *m = loader.mesh[i];
		CSkinWeights *weight = NULL;
		if(m->nFaces==0) continue;
		if(0<loader.animationset.size()) {
			if(m->skinweights.size()>0){
				weight = new CSkinWeights(loader,*m);
			}
		}
		if(0<loader.animationset.size()) {
			for(unsigned int j=0;j<m->skinweights.size();j++) {
				CFrameBone *f = (CFrameBone*)CXFrame::getFrameByName(m->skinweights[j]->transformNodeName,pFrameRoot);
				f->seMatrixOffset(m->skinweights[j]->matrixOffset);
			}
		}
		meshList.push_back(new CMesh(loader,*m,weight));
		if(m->skinweights.size()>0){
			delete weight;
		}
	}
	return true;

}
bool CModelX::Load(const char* filePath){
	CXModelLoader loader(filePath);
	return Load(loader);

}

void CModelX::UpdateAnimation(){
	if(animation) {
		animation->upDate();
		animation->updateMatrix();
		pFrameRoot->calcMatrix();
	}
}

CMatrix CModelX::getFrameMatrix(const char *name,bool local){
	CFrameBone *f = (CFrameBone*)CFrameBone::getFrameByName(name,pFrameRoot);
	return (local) ? f->getSMatrix() : getMatrix() * f->getSMatrix();
}