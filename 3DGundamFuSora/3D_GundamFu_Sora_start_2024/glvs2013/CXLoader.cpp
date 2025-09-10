#include <stdio.h>
#include <string.h>
#include "CXLoader.h"



/*
  一行読み込み
*/
char str[255]="";
#define SCAN_DATA(fp,str,format,...) {	        \
	fgets(str,255,fp);							\
	sscanf(str,format,__VA_ARGS__);				\
}

/*
  文字列から;"}{を削除
*/
static void cutSymbol(char *src,char *dst){
	while(*src) {
		if(*src != ' ' && *src != '}' && *src != '{' && *src != ';' && *src != '\"') *dst++=*src++;
		else src++;
	}
	*dst = '\0';
}

/*
  トークンを読み込む
*/
static void readToken(FILE *fp,char* token)
{
	char t[64]="";
	SCAN_DATA(fp,str,"%s",t);	
	cutSymbol(t,token);
}
/*
  ファイルから読み込む
*/
bool CXLoader::load(char *filePath) {
	FILE *fp;
	if(!(fp = fopen(filePath,"r"))) {
	}
	char token[64];
	while(!feof(fp)){
		readToken(fp,token);
		if(strcmp(token,"Mesh")==0) {
			printf("%sを発見",token);
		}
		if(strcmp(token,"MeshMaterialList")==0) {
			printf("%sを発見",token);
		}
		if(strcmp(token,"Material")==0) {
			printf("%sを発見",token);
		}
		if(strcmp(token,"MeshNormals")==0) {
			printf("%sを発見",token);
		}
	}
	return true;
}