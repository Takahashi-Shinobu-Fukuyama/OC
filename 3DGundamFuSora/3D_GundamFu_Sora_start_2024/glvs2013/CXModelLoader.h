/*
 ���t�@�C�����[�_�[
 ���t�@�C������f�[�^��ǂݍ���
*/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CMatrix.h"

#include <vector>

#define MAX_STRINGS	64

#define SAFE_DELETE(x) if((x)) delete (x); (x)=NULL;
#define SAFE_DELETE_ARRAY(x) if((x)) delete[] (x); (x)=NULL;

struct SXVector3{
	union {
		float f[3];
		struct{
			float x;
			float y;
			float z;
		};
	};
};
struct SXVector4{
	union {
		float f[4];
		struct{
			float x;
			float y;
			float z;
			float w;
		};
	};
};
struct SXIndex3{
	int idx[3];
};
struct SXIndex4{
	int idx[4];
};

struct SXColorRGB{
	float r;
	float g;
	float b;
};


struct SXColorRGBA{
	float r;
	float g;
	float b;
	float a;
};

struct SXTexCode2{
	float u;
	float v;
};
/*
 �t���[���̎��
*/
enum E_FRAME_TYPE{
	eFrame,	//�ʏ�t���[��	
	eMesh,	//���b�V���t���[��
};
/*
 �t���[���N���X
 ���t�@�[�����̊K�w�\�������̂܂܍\�z
 [Frame]
*/
class CXFrame {
protected:
	char token[MAX_STRINGS];
	char name[MAX_STRINGS];
	int	 no;			//�t���[���ԍ�
	E_FRAME_TYPE  type;	//�t���[���̎��
	CMatrix matrix;	//[FrameTransformMatrix]



	CXFrame* pChild;	//�q�t���[��
	CXFrame* pParent;	//�e�t���[��
	CXFrame* pNext;		//�Z��t���[��
	CXFrame* pPrev;
	static int frameCont; //�t���[������

	
	static CXFrame* getFrameByName_r(const char *name,CXFrame *f);
	static CXFrame* getFrameByNo_r(const int no,CXFrame *f);
public:
	CXFrame() {
		setType(eFrame);
		pChild  = NULL;
		pParent = NULL;
		pNext   = NULL;
		pPrev   = NULL;
	}
	CXFrame(char *t,char *n,int number) {
		setType(eFrame);
		pChild  = NULL;
		pParent = NULL;
		pNext   = NULL;
		pPrev   = NULL;
		no = number;
		copyToken(t);
		copyName(n);
	}
	virtual ~CXFrame(){
	}
	char* getToken() {return token;}
	void copyToken(const char* c) {
		strcpy_s(token,sizeof(token),c);
	}
	char* getName() {return name;}
	void copyName(const char* c) {
		strcpy_s(name,sizeof(name),c);
	}
	CXFrame* getChild() {return pChild;}
	void setChild(CXFrame* p) {pChild = p;}
	CXFrame* getParent() {return pParent;}
	void setParent(CXFrame* p) {pParent = p;}
	CXFrame* getNext() {return pNext;}
	void setNext(CXFrame* p) {pNext = p;}
	CXFrame* getPrev() {return pPrev;}
	void setPrev(CXFrame* p) {pPrev = p;}
	void setNo(int n) { no = n;}
	int getNo() { return no;}
	void setType(E_FRAME_TYPE t) { type = t;}
	E_FRAME_TYPE getType() { return type;}

	
	void setMatrix(CMatrix &m) {		
		matrix = m;
	}
	CMatrix getMatrix() {		
		return matrix;
	}

	CXFrame(CXFrame &f) {
		pChild  = f.getChild();
		pParent = f.getParent();
		pNext   = f.getNext();
		pPrev   = f.getPrev();
		if(pParent && pParent->getChild()==&f) pParent->setChild(this);
		if(pChild) {
			pChild->setParent(this);
			CXFrame *p = pChild->getNext();
			while(p) {
				p->setParent(this);
				p = p->getNext();
			}
		}
		if(pNext) pNext->setPrev(this);
		if(pPrev) pPrev->setNext(this);
		setNo(f.getNo());
		setMatrix(f.getMatrix());
		setType(f.getType());
		copyToken(f.getToken());
		copyName(f.getName());
	}
	/*
	  ���O����t���[�����擾
	*/
	static CXFrame* getFrameByName(const char *name,CXFrame *f);
	/*
	  �ԍ�����t���[�����擾
	*/
	static CXFrame* getFrameByNo(const int no,CXFrame *f);

	static int getFrameNomber(){
		return frameCont++;
	}
	static void resetFrameNomber(){
		frameCont = 0;
	}
	void release();
	/*
	  �f�o�b�O�p
	*/
	static void printMatrix(CXFrame *f);
	static void printFrame(CXFrame *f,int level);
	virtual void draw(){};

};
/*
   [Material]
*/
class CXMaterial:public CXFrame{
public:
	SXColorRGBA	faceColor;
	float power;
	SXColorRGB	specularColor;
	SXColorRGB	emissiveColor;
	char filename[MAX_STRINGS];
public:
	CXMaterial(FILE *fp);

};


/*
  ���b�V���p�t���[��
*/

/*
  [MeshTextureCoords]
*/
class CXMeshTextureCoords{
public:
	int nTextureCoords;
	SXTexCode2 *textureCoords;
public:
	CXMeshTextureCoords(FILE *fp);
	~CXMeshTextureCoords();
};
/*
  [MeshMaterialList]
*/
typedef char char32[32];
class CXMeshMaterialList {
public:
	int nMaterials;
	int nFaceIndexes;
	int *faceIndexes;
	char32 *material;
public:
	CXMeshMaterialList(FILE *fp);
	~CXMeshMaterialList();
};
/*
  [SkinWeights]
*/
class CXSkinWeights {
public:
	char transformNodeName[32];
	int nWeights;
	int *vertexIndices;
	float *weights;
	CMatrix matrixOffset;
public:
	CXSkinWeights(FILE *fp);
	~CXSkinWeights();

};
/*
  [MeshNormals]
*/
class CXMeshNormal {
public:
	int nNormals;
	SXVector3 *normals;
	int nFaceNormals;
	SXIndex3 *faceNormals;
public:
	CXMeshNormal(FILE *fp);
	~CXMeshNormal();


};
/*
  [Mesh]
*/
class CXMesh : public CXFrame{
public:
	int nVertices;
	SXVector3 *vertices;
	int nFaces;
	SXIndex3 *faces;


	CXMeshMaterialList *meshMaterialList;
	CXMeshNormal *meshNormal;
	CXMeshTextureCoords *meshTextureCoords;
	std::vector<CXSkinWeights*> skinweights;

	void loadVertexData(FILE *fp);

public:
	CXMesh();
	~CXMesh();
	CXMesh(CXFrame &f) : CXFrame(f){
		setType(eMesh);
		vertices = NULL;
		faces = NULL;
		nVertices = 0;
		meshMaterialList = NULL;
		meshNormal = NULL;
		meshTextureCoords = NULL;
	}
	void loadFile(FILE *fp);
		
};


/*
  �A�j���[�V�����p
*/
class CXTimedFloatKeys {
public:
	int time;
	CMatrix matrix;
};
/*
  [AnimationKey]
*/
class CXAnimationKey {
public:
	int keyType;
	int nKeys;
	CXTimedFloatKeys *keys;
public:
	CXAnimationKey(){
		keys = NULL;
	}
	CXAnimationKey(CXAnimationKey &a){
		nKeys = a.nKeys;
		keyType = a.keyType;
		keys = new CXTimedFloatKeys[nKeys];
		memcpy(keys,a.keys,sizeof(CXTimedFloatKeys)*nKeys);
	}
	CXAnimationKey(FILE *fp);
	~CXAnimationKey();

};
/*
  [Animation]
*/
class CXAnimation {
public:
	char boneName[64];
	CXAnimationKey *animationkey;
public:
	CXAnimation(){
	}
	CXAnimation(CXAnimation &a){
		strcpy_s(boneName,sizeof(boneName),a.boneName);
		animationkey = new CXAnimationKey(*a.animationkey);
	}
	CXAnimation(FILE *fp);
	~CXAnimation();
};
/*
  [AnimationSet]
*/
class CXAnimationSet : public CXFrame{
public:
	std::vector<CXAnimation*> animations;
public:
	CXAnimationSet(){
	}
	CXAnimationSet(CXAnimationSet &a){
		for(unsigned int i=0;i<a.animations.size();i++) {
			animations.push_back(new CXAnimation(*a.animations[i]));
		}
	}
	CXAnimationSet(FILE *fp);
	~CXAnimationSet();
};
/*
  x�t�@�C������K�w�\���œǂݍ���
*/
class CXModelLoader {
public:
	//�t���[�����[�g
	CXFrame* pRoot;
	//���b�V�����X�g
	std::vector<CXMesh*> mesh;  
	//�}�e���A�����X�g
	std::vector<CXMaterial*> materials; 
	//�A�j���[�V�������X�g
	std::vector<CXAnimationSet*> animationset; 

	CXFrame* readFrame(FILE *fp,CXFrame *f);
	static std::vector<CXMaterial*> *pMaterials;

public:
	CXModelLoader(const char* filePath);
	~CXModelLoader() {
		if(pRoot) pRoot->release();	
		for(std::vector<CXMaterial*>::iterator it=materials.begin();it!=materials.end();it++){
			SAFE_DELETE(*it);
		}
		materials.clear();
		for(std::vector<CXAnimationSet*>::iterator it=animationset.begin();it!=animationset.end();it++){
			SAFE_DELETE(*it);
		}
		animationset.clear();
	}
	/*
	  ���O����}�e���A���̔ԍ����擾
	*/
	int getMaterialNum(const char *name);
	/*
	  ���O����t���[���̔ԍ����擾
	*/
	int getFrameNum(const char *name);
	/*
	  �f�o�b�O�p
	*/
	static void printFrame(CXFrame *f,int level);
};
