#include "CXModelLoader.h"

int CXFrame::frameCont = 0;
std::vector<CXMaterial*> *CXModelLoader::pMaterials;

static char str[255]="";
#define SCAN_DATA(fp,str,format,...) {	\
	fgets(str,255,fp);							\
	sscanf_s(str,format,__VA_ARGS__);				\
}												\

/*
  文字列から;"を削除
*/
static void cutSymbol(char *src,char *dst){
	while(*src) {
		if(*src != ';' && *src != '\"') *dst++=*src++;
		else src++;
	}
	*dst = '\0';
}
/*
  トークン　名前 を読み込む
*/
static void readToken(FILE *fp,char* token,char* name)
{
	token[0] = '\0';
	name[0] = '\0';
	SCAN_DATA(fp,str,"%s %s\n",token,name);	
}
/*
  トークン を読み込む
*/
static void readToken(FILE *fp,char* token)
{
	token[0] = '\0';
	while(fscanf_s(fp,"%s",token,64) == 0 && !feof(fp));
}
/*
/*
 特定の文字列が出現するまでファイルポインタを進める
*/
static void forwardFilePointer(FILE *fp,char c)
{
	while(getc(fp) != c);
}
/*
 要素の読み込みを飛ばす
*/
static void skipToken(FILE *fp) {
	char token[64]="";
	char name[64]="";
	while(!feof(fp)) {
		readToken(fp,token);
		if(strcmp(token,"{")==0) {
			skipToken(fp);
		}
		if(strcmp(token,"}")==0) {
			return;
		}
	}
}
/*
  マトリックスの読み込み
*/
static void readMatrix(FILE *fp,CXFrame *f) {
	CMatrix m;
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
	for(int i=0;i<4;i++) {
		SCAN_DATA(fp,str,"%f,%f,%f,%f,\n",&m.m[i][0],&m.m[i][1],&m.m[i][2],&m.m[i][3]);
	}
	fgets(str,255,fp); //　}飛ばし

	f->setMatrix(m);
	
}

CXFrame* CXFrame::getFrameByName_r(const char *name,CXFrame *f){
	CXFrame *find=NULL;
	if(strcmp(f->getName(),name) == 0) return f;
	if(f->getChild()) find=CXFrame::getFrameByName_r(name,f->getChild());
	if(f->getNext() && !find) find=CXFrame::getFrameByName_r(name,f->getNext());
	return find;
}
CXFrame* CXFrame::getFrameByName(const char *name,CXFrame *f){
	CXFrame *pRoot=f;
	while(pRoot->getParent()) pRoot = pRoot->getParent();

	return getFrameByName_r(name,pRoot);
}
CXFrame* CXFrame::getFrameByNo_r(const int no,CXFrame *f){
	CXFrame *find=NULL;
	if(f->getNo() == no) return f;
	if(f->getChild()) find=CXFrame::getFrameByNo_r(no,f->getChild());
	if(f->getNext() && !find) find=CXFrame::getFrameByNo_r(no,f->getNext());
	return find;
}
CXFrame* CXFrame::getFrameByNo(const int no,CXFrame *f){
	CXFrame *pRoot=f;
	while(pRoot->getParent()) pRoot = pRoot->getParent();

	return getFrameByNo_r(no,pRoot);
}

void CXFrame::printFrame(CXFrame *f,int level){
	for(int i=0;i<level;i++) printf(" ");
	printf("%s %s No%d\n",f->getToken(),f->getName(),f->getNo());
	if(f->getChild()) printFrame(f->getChild(),level+1);
	if(f->getNext()) printFrame(f->getNext(),level);
}
void CXFrame::printMatrix(CXFrame *f){
	CMatrix m = f->getMatrix();
	printf("%s %s No%d\n"
		   " %f %f %f %f\n"
		   " %f %f %f %f\n"
		   " %f %f %f %f\n"
		   " %f %f %f %f\n",f->getToken(),f->getName(),f->getNo(),
		   m.m00,m.m01,m.m02,m.m03,
		   m.m10,m.m11,m.m12,m.m13,
		   m.m20,m.m21,m.m22,m.m23,
		   m.m30,m.m31,m.m32,m.m33);
	if(f->getChild()) printMatrix(f->getChild());
	if(f->getNext()) printMatrix(f->getNext());
}

void CXFrame::release(){
	if(getChild()) getChild()->release();
	if(getNext()) getNext()->release();
	delete this;
}

CXMaterial::CXMaterial(FILE *fp){
	char tmp[MAX_STRINGS];
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
	SCAN_DATA(fp,str,"%f;%f;%f;%f;;",&faceColor.r,&faceColor.g,&faceColor.b,&faceColor.a);
	SCAN_DATA(fp,str,"%f;",&power);
	SCAN_DATA(fp,str,"%f;%f;%f;;",&specularColor.r,&specularColor.g,&specularColor.b);
	SCAN_DATA(fp,str,"%f;%f;%f;;",&emissiveColor.r,&emissiveColor.g,&emissiveColor.b);
	//SCAN_DATA(fp,str,"%s",tmp,MAX_STRINGS);
	fscanf(fp,"%s",tmp);
	if(strcmp(tmp,"TextureFilename")==0){
		forwardFilePointer(fp,'\"');
		for(int i=0;i<MAX_STRINGS;i++) {
			int c = getc(fp);
			if(c == '\"') {
				filename[i] = '\0';
				break;
			}
			filename[i] = c;
		}
		forwardFilePointer(fp,'}');
		fgets(str,255,fp); //}とばし
	} else {
		filename[0] = '\0';
	}
	fgets(str,255,fp); //}とばし
}
CXMeshTextureCoords::CXMeshTextureCoords(FILE *fp) {
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
	SCAN_DATA(fp,str,"%d;",&nTextureCoords);
	textureCoords = new SXTexCode2[nTextureCoords];
	for(int i=0;i<nTextureCoords;i++) {
		SCAN_DATA(fp,str,"%f;%f;,",&textureCoords[i].u,&textureCoords[i].v);
	}
	fgets(str,255,fp); //}とばし
}
CXMeshTextureCoords::~CXMeshTextureCoords() {
	SAFE_DELETE_ARRAY(textureCoords);
}
CXMeshMaterialList::CXMeshMaterialList(FILE *fp){
	int k=0,j=0,i=0;
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
	SCAN_DATA(fp,str,"%d",&nMaterials);
	
	SCAN_DATA(fp,str,"%d",&nFaceIndexes);
	faceIndexes = new int[nFaceIndexes];
	for(i=0;i<nFaceIndexes;i++) {
		SCAN_DATA(fp,str,"%d",&faceIndexes[i]);
	}
	material = new char32[nMaterials];
	for(i=0;i<nMaterials;i++) {
		char token[64]="";
		char name[64]="";
		fscanf_s(fp,"%s",token,64);
//		sscanf_s(str,"%s %s",token,64,name,64);
		if(strcmp(token,"Material") == 0 ){
			fscanf_s(fp,"%s",name,64);
			//Mesh内にマテリアルが定義されている
			CXMaterial *m = new CXMaterial(fp);
			m->copyToken(token);
			m->copyName(name);
			CXModelLoader::pMaterials->push_back(m);
			strcpy_s(material[i],32,name);

		} else {
			//ファイルの先頭でマテリアルが定義されている場合
			for(j=0,k=0;token[j]!='}';j++) {
				if(token[j]!='{' && token[j]!=' ') {
					material[i][k++]=token[j];
				}
			}
			material[i][k]='\0';
		}

	}
	
	forwardFilePointer(fp,'}');
	fgets(str,255,fp);
	//fgets(str,255,fp); //}とばし

}
CXMeshMaterialList::~CXMeshMaterialList() {
	SAFE_DELETE_ARRAY(faceIndexes);
	SAFE_DELETE_ARRAY(material);
}
CXMeshNormal::CXMeshNormal(FILE *fp) {
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
	SCAN_DATA(fp,str,"%d",&nNormals);

	normals = new SXVector3[nNormals];
	for(int i=0;i<nNormals;i++) {
		fgets(str,255,fp);
		sscanf_s(str,"%f;%f;%f;,",&normals[i].x,&normals[i].y,&normals[i].z);
	}
	
	SCAN_DATA(fp,str,"%d",&nFaceNormals);

	faceNormals = new SXIndex3[nFaceNormals];
	for(int i=0;i<nFaceNormals;i++) {
		int dummy;
		SCAN_DATA(fp,str,"%d;%d,%d,%d;,",&dummy,&faceNormals[i].idx[0],&faceNormals[i].idx[1],&faceNormals[i].idx[2]);
	}
	fgets(str,255,fp); //}とばし
}
CXMeshNormal::~CXMeshNormal(){	
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(faceNormals);
}
CXSkinWeights::CXSkinWeights(FILE *fp) {
	char tmp[MAX_STRINGS];
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
	SCAN_DATA(fp,str,"%s",tmp,MAX_STRINGS);
	cutSymbol(tmp,transformNodeName);
	SCAN_DATA(fp,str,"%d;",&nWeights);

	if(nWeights==0) {
		vertexIndices=NULL;
		weights = NULL;
	} else {
		vertexIndices=new int[nWeights];
		weights=new float[nWeights];
		for(int i=0;i<nWeights;i++) {
			SCAN_DATA(fp,str,"%d",&vertexIndices[i]);
		}
		for(int i=0;i<nWeights;i++) {
			SCAN_DATA(fp,str,"%f",&weights[i]);
		}
	}
	for(int i=0;i<4;i++) {
		char *tmp=str;
		SCAN_DATA(fp,str,"%f,%f,%f,%f,\n",&matrixOffset.m[i][0],&matrixOffset.m[i][1],&matrixOffset.m[i][2],&matrixOffset.m[i][3]);
		if(i==0) {
			//;とばし
			while(*tmp) {
				if(*tmp++==';') {
					i--;
					break;
				}
			}
		}
	}
	fgets(str,255,fp); //}とばし
}

CXSkinWeights::~CXSkinWeights() {
	SAFE_DELETE_ARRAY(vertexIndices);
	SAFE_DELETE_ARRAY(weights);
}

void CXMesh::loadVertexData(FILE *fp) {

	SCAN_DATA(fp,str,"%d",&nVertices);

	vertices = new SXVector3[nVertices];
	for(int i=0;i<nVertices;i++) {
		SCAN_DATA(fp,str,"%f;%f;%f;,",&vertices[i].x,&vertices[i].y,&vertices[i].z);
	}
	
	SCAN_DATA(fp,str,"%d",&nFaces);

	faces = new SXIndex3[nFaces];
	for(int i=0;i<nFaces;i++) {
		int dummy;
		SCAN_DATA(fp,str,"%d;%d,%d,%d;,",&dummy,&faces[i].idx[0],&faces[i].idx[1],&faces[i].idx[2]);
	}
}

CXMesh::CXMesh(){	
	setType(eMesh);
	vertices = NULL;
	faces = NULL;
	nVertices = 0;
	meshMaterialList = NULL;
	meshNormal = NULL;
	meshTextureCoords = NULL;
}
CXMesh::~CXMesh(){
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(faces);
	SAFE_DELETE(meshNormal);
	SAFE_DELETE(meshMaterialList);
	SAFE_DELETE(meshTextureCoords);
	for(std::vector<CXSkinWeights*>::iterator it=skinweights.begin();it!=skinweights.end();it++){
		SAFE_DELETE(*it);
	}
	skinweights.clear();		
}
void CXMesh::loadFile(FILE *fp) {
	
	char token[MAX_STRINGS];
//	char name[MAX_STRINGS];
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);

	loadVertexData(fp);
	while(1) {
		//fscanf_s(fp,"%s ",token);
		readToken(fp,token);
		if(strcmp(token,"MeshTextureCoords")==0) {
			meshTextureCoords = new CXMeshTextureCoords(fp);
		}else
		if(strcmp(token,"MeshNormals")==0) {
			meshNormal = new CXMeshNormal(fp);
		} else if(strcmp(token,"SkinWeights")==0) {
			CXSkinWeights *sw = new CXSkinWeights(fp);
			skinweights.push_back(sw);

		} else 
		if(strcmp(token,"MeshMaterialList")==0) {
			meshMaterialList = new CXMeshMaterialList(fp);
		} else
		if(strcmp(token,"}")==0) {
			fgets(str,255,fp); // }とばし
			break;
		} else {
			forwardFilePointer(fp,'{');
			skipToken(fp);
		}
	}


}
CXAnimationKey::CXAnimationKey(FILE *fp) {
	char token[32];
	keys = NULL;
	while(1) {
		SCAN_DATA(fp,str,"%d;",&keyType);
		SCAN_DATA(fp,str,"%d;",&nKeys);
		if(!keys) keys = new CXTimedFloatKeys[nKeys];
		int dummy;
		float x,y,z,w;
		for(int i=0;i<nKeys;i++) {
			CMatrix tmp,matRX,matRY,matRZ;
			fscanf(fp,"%d;%d;\n",&keys[i].time,&dummy);
			switch(keyType) {
			// Rotation
			case 0:
				SCAN_DATA(fp,str,"%f,%f,%f,%f,\n",&w,&x,&y,&z);
				tmp.setQuaternion(x,y,z,w);
				/*
				matRX.RotationX(x);
				matRY.RotationY(y);
				matRZ.RotationZ(z);
				tmp = matRZ*matRY*matRX;
				*/
//				keys[i].matrix = keys[i].matrix * tmp;
				keys[i].matrix = tmp * keys[i].matrix;
				break;
			//Scale
			case 1:
				SCAN_DATA(fp,str,"%f,%f,%f,\n",&x,&y,&z);
				tmp.Scale(x, y, z);
//				keys[i].matrix = keys[i].matrix * tmp;
				keys[i].matrix = tmp * keys[i].matrix;
				break;
			//Position
			case 2:
				SCAN_DATA(fp,str,"%f,%f,%f,\n",&x,&y,&z);
				tmp.Transelate(x,y,z);
//				keys[i].matrix = keys[i].matrix * tmp;
				keys[i].matrix = tmp * keys[i].matrix;

				break;
			//Matrix
			case 4:
				for(int j=0;j<4;j++) {
					SCAN_DATA(fp,str,"%f,%f,%f,%f,\n",	&keys[i].matrix.m[j][0],
														&keys[i].matrix.m[j][1],
														&keys[i].matrix.m[j][2],
														&keys[i].matrix.m[j][3]);
				}
				break;
			default:
				fgets(str,255,fp); // とばし
				break;
			}
		}
		fgets(str,255,fp); // }とばし
		fscanf_s(fp,"%s",token,32);
		//AnimationKey再読み込み
		if(strcmp(token,"AnimationKey")==0) {
			fgets(str,255,fp); // {とばし
			continue;
		}
		break;
	}
}
CXAnimationKey::~CXAnimationKey(){
	SAFE_DELETE_ARRAY(keys);
}
CXAnimation::CXAnimation(FILE *fp) {
	
	forwardFilePointer(fp,'{');
	fgets(str,255,fp); // {とばし
	//ボーン名読み込み
	fgets(str,255,fp);
//	fgets(str,255,fp);
	int i,k;
	//ボーン名取得
	for(i=0,k=0;str[i]!='}';i++) {
		if(str[i]!='{' && str[i]!=' ') {
			boneName[k++]=str[i];
		}
	}
	boneName[k]='\0';

	// AnimationKeyとばし
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
//	fgets(str,255,fp);

	animationkey = new CXAnimationKey(fp);
//	fgets(str,255,fp); // }とばし
}
CXAnimation::~CXAnimation(){
	SAFE_DELETE(animationkey);
}
CXAnimationSet::CXAnimationSet(FILE *fp) {
	char token[MAX_STRINGS];
//	char name[MAX_STRINGS];
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
//	fgets(str,255,fp); // {とばし
	while(1) {
		readToken(fp,token);
		if(strcmp(token,"Animation")==0) {
			CXAnimation *a = new CXAnimation(fp);
			animations.push_back(a);
		}else
		if(strcmp(token,"}")==0) {
			fgets(str,255,fp);
			break;
		}

	}

}
CXAnimationSet::~CXAnimationSet(){
	for(std::vector<CXAnimation*>::iterator it=animations.begin();it!=animations.end();it++){
		SAFE_DELETE(*it);
	}
	animations.clear();
}
CXFrame* CXModelLoader::readFrame(FILE *fp,CXFrame *f) {
	char token[64]="";
	char name[64]="";
	CXFrame *pTmpChild=NULL;
	
	forwardFilePointer(fp,'{');
	fgets(str,255,fp);
	while(!feof(fp)) {
		//strcpy(token[1],token[0]);
		//strcpy(name[1],name[0]);
		//readToken(fp,token[0],name[0]);
		readToken(fp,token);
		if(strcmp(token,"FrameTransformMatrix")==0) {
			readMatrix(fp,f);
		}else
		if(strcmp(token,"Frame")==0) {
			readToken(fp,name);
			CXFrame *c = new CXFrame(token,name,CXFrame::getFrameNomber());
			if(!f->getChild()) f->setChild(c);
			c->setParent(f);
			if(pTmpChild) {
				pTmpChild->setNext(c);
				c->setPrev(pTmpChild);
			}
			pTmpChild = readFrame(fp,c);
		}else
		if(strcmp(token,"Mesh")==0) {
			CXMesh *m = new CXMesh(*f);
			delete f;
			f=m;
			m->loadFile(fp);
			mesh.push_back(m);
		}else
		if(strcmp(token,"}")==0) {
			//次の行へ
			fgets(str,255,fp);
			break;
		} else {
			skipToken(fp);
		}
		
	}
	return f;

}

CXModelLoader::CXModelLoader(const char* filePath){
	pRoot = NULL;
	FILE* fp;
	int level = 0 ;

	if(!(fp=fopen(filePath,"r"))) {
		printf("ファイルをオープンできませんでした %s\n",filePath);
		return;
	}
	char token[64];
	char name[64];
	CXFrame::resetFrameNomber();
	CXModelLoader::pMaterials = &materials;
	while(!feof(fp)) {
		readToken(fp,token);
		printf("token %s\n",token);
		if(strcmp(token,"template")==0) {
			//テンプレートは飛ばす
			forwardFilePointer(fp,'}');
		}else
		if(strcmp(token,"Frame")==0) {
			readToken(fp,name);
			pRoot = new CXFrame(token,name,CXFrame::getFrameNomber());
			//readToken(fp,token,name);
			readFrame(fp,pRoot);
		}else
		if(strcmp(token,"Material")==0) {
			readToken(fp,name);
			CXMaterial *m = new CXMaterial(fp);
			m->copyToken(token);
			m->copyName(name);
			materials.push_back(m);
		}else
		if(strcmp(token,"AnimationSet")==0) {
			readToken(fp,name);
			CXAnimationSet *a = new CXAnimationSet(fp);
			a->copyToken(token);
			a->copyName(name);
			animationset.push_back(a);
		}
	}
	if(!pRoot) {
		printf("Frameが存在しません%s\n",filePath);
		return;
	}


	fclose(fp);
	printf("\nprint frame\n\n");
	CXFrame::printFrame(pRoot,0);
	
	printf("animationset %d\n",animationset.size());
}

int CXModelLoader::getMaterialNum(const char *name){
	for(unsigned int i=0;i<materials.size();i++) {
		if(strcmp(materials[i]->getName(),name)==0) return i;
	}
	return -1;
}

int CXModelLoader::getFrameNum(const char *name){
	CXFrame *f = CXFrame::getFrameByName(name,pRoot);
	return f->getNo();
}