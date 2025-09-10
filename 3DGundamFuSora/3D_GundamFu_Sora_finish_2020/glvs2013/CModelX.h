
/*
  X�t�@�C���̃��f���N���X
*/
#pragma once

#include "CXModelLoader.h"
#include "CTexture.h"
#include "CShader.h"

#include "glew.h"
#include "glut.h"

#include "Vertex.h"
#include "CModel.h"
/*
  ���f���̊K�w�\���p�N���X
  CXFrame���X�L�����b�V���p�ɔh��
*/
class CFrameBone : public CXFrame{
private:
	CMatrix boneMatrix;
	CMatrix sMatrix;
	CMatrix matrixOffset;
public:
	CFrameBone():CXFrame(){
	}
	CFrameBone(CXFrame &f){
		setNo(f.getNo());
		setMatrix(f.getMatrix());
		setType(f.getType());
		copyToken(f.getToken());
		copyName(f.getName());
	}
	CMatrix getSMatrix(){
		return sMatrix;

	}
	void calcBoneMatrix(CMatrix *out);
	void calcMatrix();
	CMatrix getMatrixOffset(){
		return matrixOffset ;
	}
	void seMatrixOffset(CMatrix mat){
		matrixOffset = mat;
	}
};
/*
  �A�j���[�V�����Ǘ��N���X
  �ǂݍ��񂾃A�j���[�V�����\�������̂܂܎g�p
*/
class CAnimation {
	std::vector<CXAnimationSet*> animation;
	float time;
	int jam;
	float speed;
	bool loop;
	int endTime;
	CFrameBone *pBoneRoot;
private:
	CMatrix calcMatrix(CXAnimationKey &key,float t);
public:
	CAnimation();
	CAnimation(std::vector<CXAnimationSet*> &a);
	~CAnimation();
	int getEndTime(int j);
	void changeAnimation(int j,bool l = true,bool check=true);
	void upDate();
	void updateMatrix();
	bool isEnd(){
		if(time >= endTime) return true;
		return false;
	}
	int getJam() {
		return jam;
	}
	void setSpeed(float s) {
		speed = s;
	}
	void attachFrame(CFrameBone *bone){
		pBoneRoot = bone;
	}
	
};
/*
  �}�e���A���N���X
*/
class CMaterial {
public:
	SXColorRGBA	faceColor;
	float power;
	SXColorRGB	specularColor;
	SXColorRGB	emissiveColor;
	CTexture	*texture;
public:
	CMaterial(CXMaterial &m);
	~CMaterial();
	void set(CShader *shader);
	void reset();
};
struct SSkinWeight {
	SXVector4 bone;
	SXVector4 weight;
};


/*
  �e���_�ɐݒ肳�ꂽ�E�F�C�g�l�𒸓_���ɂ܂Ƃ߂�N���X
*/
class CSkinWeights {
public:
	SSkinWeight *weight;
public:
	CSkinWeights( CXModelLoader &loader, CXMesh &mesh);
	~CSkinWeights();
};
/*
  �|���S���N���X
  �}�e���A�����ɂ܂Ƃ߂�
*/
class CPoly{
public:
	MY_VERTEX  *m_pVertex;
	int			vertexCnt;
	int			faceCnt;
	bool		bNormal;
	bool		bTexCode;
	bool		bWeight;
	int			material;
	GLuint		buffer;
public:
	/*
	  ���[�_�[����|���S���f�[�^���\�z
	*/
	CPoly( CXModelLoader &loader,  CXMesh &mesh,CSkinWeights *bone,int matNo);
	~CPoly();
	/*
	  �`��
	*/
	void draw(CShader *shader);
};
/*
  ���b�V�����̃{�[���I�t�Z�b�g�s����i�[
*/
class CBoneOffset{
public:
	char transformNodeName[64];
	CMatrix matrixOffset;
	CBoneOffset(){
	}
	/*
	  SkinWeights����{�[�����ƃ}�g���b�N�X�݂̂��i�[
	*/
	CBoneOffset(CXSkinWeights &s){
		strcpy_s(transformNodeName,sizeof(transformNodeName),s.transformNodeName);
		matrixOffset = s.matrixOffset;
	}

};
/*
  ���b�V���N���X
*/
class CMesh{
private:
	std::vector<CPoly*> polyList;
	std::vector<CBoneOffset*> boneList;
	CMatrix matrix;
public:
	/*
	  ���[�_�[���烁�b�V���f�[�^���\�z
	*/
	CMesh(CXModelLoader &loader,  CXMesh &mesh,CSkinWeights *bone);
	~CMesh();
	/*
	  �`��
	*/
	void draw(std::vector<CMaterial*> materialList,CShader *shader,CFrameBone *pFrameRoot,CMatrix *boneMatrix,int boneNum);

};

/*
  ���t�@�C���p���f���N���X
*/
class CModelX : public CModel{
private:
	std::vector<CMesh*> meshList;
	std::vector<CMaterial*> materialList;
	CFrameBone *pFrameRoot;
	int			boneNum;
	CMatrix *boneMatrix;
	CAnimation *animation;
	void createFrame(CXFrame *xf,CFrameBone *f,int *num);
	CShader		*shader;
public:
	CModelX();
	~CModelX();
	void Release();
	/*
	  ���[�_�[���烂�f�����\�z
	*/
	bool Load(CXModelLoader &loader);
	/*
	  �t�@�C�����烂�f�����\�z
	*/
	bool Load(const char* filePath);
	/*
	  �A�j���[�V�������X�V
	*/
	void UpdateAnimation();
	/*
	  �A�j���[�V�����̏I���𔻒�
	*/
	bool isAnimationEnd(){
		if(!animation) return true;
		return animation->isEnd();
	}
	/*
	  �A�j���[�V�����̎�ނ��擾
	*/
	int getAnimationJam(){
		if(!animation) return true;
		return animation->getJam();
	}
	/*
	�@�A�j���[�V�������x�̕ύX
	*/
	void setAnimationSpeed(float s) {
		animation->setSpeed(s);
	}
	/*
	  �A�j���[�V������؂�ւ���
	*/
	void ChangeAnimation(int jam,bool loop = true,bool check=true){
		if(!animation) return;
		animation->changeAnimation(jam,loop,check);
	}

	/*
	  �`��
	*/
	void RenderM(CMatrix &m);

	/*
	�@�{�[���̍s����擾
	*/
	CMatrix getFrameMatrix(const char *name,bool local=false);

	std::vector<CMaterial*> getMaterialList() { return materialList; }


};