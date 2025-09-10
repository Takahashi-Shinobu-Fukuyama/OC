#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CXSModelLoader.h"

static char str[255]="";
static int scannum=0;
#define SCAN_DATA(fp,format,...) {	\
	fgets(str,255,fp);							\
	scannum=sscanf_s(str,format,__VA_ARGS__);				\
}												\
/*
  文字列から;"を削除
*/
static void cutSymbol(char *src,char *dst){
	while(*src) {
		if(*src != ';' && *src != '\"' && *src != '}' && *src != '{') *dst++=*src++;
		else src++;
	}
	*dst = '\0';
}

CXSModelLoader::CXSModelLoader(const char* filePath){
	FILE *fp;
	if(!(fp=fopen(filePath,"r"))) {
		printf("ファイルをオープンできませんでした %s\n",filePath);
		return;
	}
	m_vertex = NULL;
	m_vertexIndex = NULL;
	m_normal = NULL;
	m_normalIndex = NULL;
	m_texCoord = NULL;
	m_material = NULL;
	m_materialList = NULL;
	char token[64];
	int  mterialCnt=0;
	while(!feof(fp)) {
		token[0] = 0;
		SCAN_DATA(fp,"%s",token,64);
		if(strcmp(token,"Mesh")==0) {
			SCAN_DATA(fp,"%d;",&m_vertexNum);
			m_vertex = new CXSVector3[m_vertexNum];
			for(int i=0;i<m_vertexNum;i++) {
				SCAN_DATA(fp,"%f;%f;%f;,",&m_vertex[i].x,&m_vertex[i].y,&m_vertex[i].z);
			}
			SCAN_DATA(fp,"%d;",&m_faceNum);
			if(scannum<=0)	//一行空改行がある場合
				SCAN_DATA(fp,"%d;",&m_faceNum);
			m_vertexIndex = new int[m_faceNum*3];
			int dummy;
			int *idx = m_vertexIndex;
			for(int i=0;i<m_faceNum;i++,idx+=3) {
				SCAN_DATA(fp,"%d;%d,%d,%d;,",&dummy,&idx[0],&idx[1],&idx[2]);
			}
		}
		if(strcmp(token,"MeshMaterialList")==0) {
			SCAN_DATA(fp,"%d;",&m_materialNum);
			fgets(str,255,fp);	//空読み
			m_material = new CXSMaterial[m_materialNum];
			m_materialList = new int[m_faceNum];
			for(int i=0;i<m_faceNum;i++) {
				SCAN_DATA(fp,"%d;",&m_materialList[i]);
			}
		}
		if(strcmp(token,"Material")==0) {
			CXSMaterial *m = &m_material[mterialCnt++];
			SCAN_DATA(fp,"%f;%f;%f;%f;;",&m->faceColor.r,&m->faceColor.g,&m->faceColor.b,&m->faceColor.a);
			SCAN_DATA(fp,"%f;",&m->power);
			SCAN_DATA(fp,"%f;%f;%f;;",&m->specularColor.r,&m->specularColor.g,&m->specularColor.b);
			SCAN_DATA(fp,"%f;%f;%f;;",&m->emissiveColor.r,&m->emissiveColor.g,&m->emissiveColor.b);
			char path[32]={0};
			SCAN_DATA(fp,"%s {\"%s\";}",token,64,path,32);
			if(strcmp(token,"TextureFilename")==0) {
				if(!path[0]) {
					//ファイル名が改行後の場合
					SCAN_DATA(fp,"%s;",path,32);
				}
				cutSymbol(path,m->filePath);
			//	strcpy(m->filePath,path);
			}
			fgets(str,255,fp);	//空読み
		

		}
		if(strcmp(token,"MeshNormals")==0) {
			SCAN_DATA(fp,"%d;",&m_normalNum);
			m_normal = new CXSVector3[m_normalNum];
			for(int i=0;i<m_normalNum;i++) {
				SCAN_DATA(fp,"%f;%f;%f;,",&m_normal[i].x,&m_normal[i].y,&m_normal[i].z);
			}
			fgets(str,255,fp);	//空読み
			m_normalIndex = new int[m_faceNum*3];
			int dummy;
			int *idx = m_normalIndex;
			for(int i=0;i<m_faceNum;i++,idx+=3) {
				SCAN_DATA(fp,"%d;%d,%d,%d;,",&dummy,&idx[0],&idx[1],&idx[2]);
			}
		}
		if(strcmp(token,"MeshTextureCoords")==0) {
			fgets(str,255,fp);	//空読み
			m_texCoord = new CXSTexCoord[m_vertexNum];
			for(int i=0;i<m_vertexNum;i++) {
				SCAN_DATA(fp,"%f;%f;,",&m_texCoord[i].u,&m_texCoord[i].v);
			}
		}
	}
	fclose(fp);
}
CXSModelLoader::~CXSModelLoader(){
	delete[] m_vertex;
	delete[] m_vertexIndex;
	delete[] m_normal;
	delete[] m_normalIndex;
	delete[] m_texCoord;
	delete[] m_material;
	delete[] m_materialList;
}