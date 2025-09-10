#include "CModelObj.h"
#include <string.h>

CMaterialObj::CMaterialObj() : m_pTex(NULL){
}
void CMaterialObj::Map() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);

	if(m_pTex) {
		m_pTex->MapTexture();
	}
}

void CMaterialObj::Unmap() {
	if(m_pTex) {
		m_pTex->UnmapTexture();
	}
}

void CMaterialObj::Release() {
	if(m_pTex) {m_pTex->Release();m_pTex=NULL;}
}

bool CModelObj::LoadMaterial(char *path) {
	char key[32];
	FILE* fp = fopen(path,"r");
	//ファイルが開けなかった場合はエラーを表示しfalseを返す
	if(fp==NULL) {
		printf("ファイルがありません%s\n",path);
		return false;
	}
	m_material.m_pTex = NULL;
	fscanf(fp, "%s", key);
	while (!feof(fp)) {
		if(strcmp(key,"Ka")==0) {
			fscanf(fp,"%f %f %f",&m_material.m_ambient[0],&m_material.m_ambient[1],&m_material.m_ambient[2]);
			m_material.m_ambient[3] = 1.0f;
		} else 
		if(strcmp(key,"Kd")==0) {
			fscanf(fp,"%f %f %f",&m_material.m_diffuse[0],&m_material.m_diffuse[1],&m_material.m_diffuse[2]);
			m_material.m_diffuse[3] = 1.0f;
		} else 
		if(strcmp(key,"Ks")==0) {
			fscanf(fp,"%f %f %f",&m_material.m_specular[0],&m_material.m_specular[1],&m_material.m_specular[2]);
			m_material.m_specular[3] = 1.0f;
		}else
		if(strcmp(key,"Ns")==0) {
			fscanf(fp,"%f",&m_material.m_shininess);
		} else 
		if(strcmp(key,"d")==0) {
			float aplha;
			fscanf(fp,"%f",&aplha);
			m_material.m_ambient[3] = aplha;
			m_material.m_diffuse[3] = aplha;
			m_material.m_specular[3] = aplha;
		} else
		if(strcmp(key,"map_Kd")==0) {
			char str[128];
			fscanf(fp,"%s\n",str);
			m_material.m_pTex = new CTexture();
			m_material.m_pTex->Load(str);
		}
		fscanf(fp, "%s", key);
	}
	return true;
}

CModelObj::CModelObj() : 
m_pVertexAry(NULL),m_pNormalAry(NULL),m_pTexCoordAry(NULL),m_vertexNum(0){
}
CModelObj::~CModelObj() {
	Release();
}
bool CModelObj::Load(const char *path) {
	//ファイルポインタ(fp)の作成し開いたファイルのハンドルを渡す。
	FILE* fp = fopen(path,"r");
	//ファイルが開けなかった場合はエラーを表示しfalseを返す
	if(fp==NULL) {
		printf("ファイルがありません%s\n",path);
		return false;
	}
	//座標データ用カウンター
	int	VCnt=0;
	//法線データ用カウンター
	int	NCnt=0;
	//テクスチャーコード用カウンター
	int	TCnt=0;
	//面データ用カウンター
	int	FCnt=0;
	//キーワード格納用
	char key[32];
	//ファイルの末尾でなければ繰り返す
	while(!feof(fp)) {
		//keyをクリア
		key[0]=0;
		//1単語　文字列を読み込み
		fscanf(fp,"%s",key);
		//読み込んだキーワードが「v」なら座標データカウンターをカウントアップ
		if(strcmp(key,"v")==0) {
			VCnt++;
		} else 
		//読み込んだキーワードが「f」なら面データカウンターをカウントアップ
		if(strcmp(key,"f")==0) {
			FCnt++;
		} else 
		//読み込んだキーワードが「vn」なら法線データカウンターをカウントアップ
		if(strcmp(key,"vn")==0) {
			NCnt++;
		} else 
		//読み込んだキーワードが「vt」ならテクスチャーコードデータカウンターをカウントアップ
		if(strcmp(key,"vt")==0) {
			TCnt++;
		} else 
		//読み込んだキーワードが「mtllib」ならマテリアルファイルの読み込み
		if(strcmp(key,"mtllib")==0) {
			char str[32];
			fscanf(fp,"%s",str);
			LoadMaterial(str);
		}
	}
	printf("VCnt %d\n",VCnt);

	//座標データの数だけ配列を作成
	CVector3D *pVertex = new CVector3D[VCnt];
	//法線データの数だけ配列を作成
	CVector3D *pNormal = new CVector3D[NCnt];
	//テクスチャーコードデータの数だけ配列を作成
	CVector2D *pTexCoord = new CVector2D[TCnt];
	//描画用の頂点データの頂点数を計算　3*面の数
	m_vertexNum = 3*FCnt;
	//描画用頂点配列を作成
	m_pVertexAry = new CVector3D[m_vertexNum];
	//描画用法線配列を作成
	m_pNormalAry = new CVector3D[m_vertexNum];
	//描画用テクスチャーコード配列を作成
	m_pTexCoordAry = new CVector2D[m_vertexNum];
	//読み込んだ座標データを0から順番に格納するため、カウンターを0に戻す
	VCnt=0;
	NCnt=0;
	TCnt=0;
	//読み書きの位置をファイルの先頭に戻す
	fseek(fp,0,SEEK_SET);
	//描画用頂点配列への座標データ格納先番号
	int idx = 0;
	//ファイルの末尾でなければ繰り返す
	while(!feof(fp)) {
		//keyをクリア
		key[0]=0;
		//キーワードの読み込み
		fscanf(fp,"%s",key);
		//「ｖ」を読み込んだら、座標データを読み込む
		if(strcmp(key,"v")==0) {
			fscanf(fp,"%f %f %f",&pVertex[VCnt].x,&pVertex[VCnt].y,&pVertex[VCnt].z);
			//読み込み先を次のデータへ
			VCnt++;
		}else//「vn」を読み込んだら、法線データを読み込む
		if(strcmp(key,"vn")==0) {
			fscanf(fp,"%f %f %f",&pNormal[NCnt].x,&pNormal[NCnt].y,&pNormal[NCnt].z);
			//読み込み先を次のデータへ
			NCnt++;
		}else//「vt」を読み込んだら、テクスチャーコードデータを読み込む
		if(strcmp(key,"vt")==0) {
			fscanf(fp,"%f %f",&pTexCoord[TCnt].x,&pTexCoord[TCnt].y);
			pTexCoord[TCnt].y = 1.0f - pTexCoord[TCnt].y;
			//読み込み先を次のデータへ
			TCnt++;
		}else
		if(strcmp(key,"f")==0) {
			int v1,v2,v3;
			int n1,n2,n3;
			int t1,t2,t3;
			if (TCnt > 0) {
				fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
				m_pVertexAry[idx] = pVertex[v1 - 1];			//頂点１の座標データ
				m_pVertexAry[idx + 1] = pVertex[v2 - 1];		//頂点２の座標データ
				m_pVertexAry[idx + 2] = pVertex[v3 - 1];

				m_pNormalAry[idx] = pNormal[n1 - 1];			//頂点１の法線データ
				m_pNormalAry[idx + 1] = pNormal[n2 - 1];		//頂点２の法線データ
				m_pNormalAry[idx + 2] = pNormal[n3 - 1];

				m_pTexCoordAry[idx] = pTexCoord[t1 - 1];			//頂点１のテクスチャーコードデータ
				m_pTexCoordAry[idx + 1] = pTexCoord[t2 - 1];		//頂点２のテクスチャーコードデータ
				m_pTexCoordAry[idx + 2] = pTexCoord[t3 - 1];
			}
			else {
				fscanf(fp, "%d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
				m_pVertexAry[idx] = pVertex[v1 - 1];			//頂点１の座標データ
				m_pVertexAry[idx + 1] = pVertex[v2 - 1];		//頂点２の座標データ
				m_pVertexAry[idx + 2] = pVertex[v3 - 1];

				m_pNormalAry[idx] = pNormal[n1 - 1];			//頂点１の法線データ
				m_pNormalAry[idx + 1] = pNormal[n2 - 1];		//頂点２の法線データ
				m_pNormalAry[idx + 2] = pNormal[n3 - 1];

			}
			
			//データの格納先を3つ移動する
			idx+=3;
		}
	}

	//一時データ解放
	delete[] pVertex;
	delete[] pNormal;
	delete[] pTexCoord;
	return true;
}

void CModelObj::RenderM(CMatrix &m) {
//	UpdateMatrix();

	glPushMatrix();
//	glMultMatrixf(m_matrix.f);
	glMultMatrixf(m.f);

	m_material.Map();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3,GL_FLOAT,0,m_pVertexAry);
	glNormalPointer(GL_FLOAT,0,m_pNormalAry);

	if(m_pTexCoordAry != NULL) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,0,m_pTexCoordAry);
	}

	glDrawArrays(GL_TRIANGLES,0,m_vertexNum);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_material.Unmap();

	glPopMatrix();

}

void CModelObj::Release() {
	if(m_pVertexAry)  {delete[] m_pVertexAry; m_pVertexAry=NULL;}
	if(m_pNormalAry)  {delete[] m_pNormalAry; m_pNormalAry=NULL;}
	if(m_pTexCoordAry)  {delete[] m_pTexCoordAry; m_pTexCoordAry=NULL;}
}