#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "CCube.h"

static SVector3D vertex[] = {

	{-1.00000,1.00000,-1.00000},	//0
	{1.00000,1.00000,-1.00000},		//1
	{1.00000,-1.00000,-1.00000},	//2
	{-1.00000,-1.00000,-1.00000},	//3
	
	{1.00000,1.00000,-1.00000},		//1
	{1.00000,1.00000,1.00000},		//4
	{1.00000,-1.00000,1.00000},		//5
	{1.00000,-1.00000,-1.00000},	//2
	
	{1.00000,1.00000,1.00000},		//4
	{-1.00000,1.00000,1.00000},		//6
	{-1.00000,-1.00000,1.00000},	//7
	{1.00000,-1.00000,1.00000},		//5

	{-1.00000,1.00000,1.00000},		//6
	{-1.00000,1.00000,-1.00000},	//0
	{-1.00000,-1.00000,-1.00000},	//3
	{-1.00000,-1.00000,1.00000},	//7
	
	{-1.00000,1.00000,1.00000},		//6
	{1.00000,1.00000,1.00000},		//4
	{1.00000,1.00000,-1.00000},		//1
	{-1.00000,1.00000,-1.00000},	//0
	
	{-1.00000,-1.00000,-1.00000},	//3
	{1.00000,-1.00000,-1.00000},	//2
	{1.00000,-1.00000,1.00000},		//5
	{-1.00000,-1.00000,1.00000},		//7

	{-1.00000,3.00000,-1.00000},	//0
	{1.00000,3.00000,-1.00000},		//1
	{1.00000,1.00000,-1.00000},	//2
	{-1.00000,1.00000,-1.00000},	//3

	{1.00000,3.00000,-1.00000},		//1
	{1.00000,3.00000,1.00000},		//4
	{1.00000,1.00000,1.00000},		//5
	{1.00000,1.00000,-1.00000},	//2
	
	{1.00000,3.00000,1.00000},		//4
	{-1.00000,3.00000,1.00000},		//6
	{-1.00000,1.00000,1.00000},	//7
	{1.00000,1.00000,1.00000},		//5

	{-1.00000,3.00000,1.00000},		//6
	{-1.00000,3.00000,-1.00000},	//0
	{-1.00000,1.00000,-1.00000},	//3
	{-1.00000,1.00000,1.00000},	//7
	
	{-1.00000,3.00000,1.00000},		//6
	{1.00000,3.00000,1.00000},		//4
	{1.00000,3.00000,-1.00000},		//1
	{-1.00000,3.00000,-1.00000},	//0
};

static SVector3D normal[] = {
	{0.000000,0.000000,-1.000000},		//0
	{0.000000,0.000000,-1.000000},		//0
	{0.000000,0.000000,-1.000000},		//0
	{0.000000,0.000000,-1.000000},		//0

	{1.000000,0.000000,0.000000},		//1
	{1.000000,0.000000,0.000000},		//1
	{1.000000,0.000000,0.000000},		//1
	{1.000000,0.000000,0.000000},		//1

	{0.000000,0.000000,1.000000},		//2
	{0.000000,0.000000,1.000000},		//2
	{0.000000,0.000000,1.000000},		//2
	{0.000000,0.000000,1.000000},		//2

	{-1.000000,0.000000,0.000000},		//3
	{-1.000000,0.000000,0.000000},		//3
	{-1.000000,0.000000,0.000000},		//3
	{-1.000000,0.000000,0.000000},		//3

	{0.000000,1.000000,0.000000},		//4
	{0.000000,1.000000,0.000000},		//4
	{0.000000,1.000000,0.000000},		//4
	{0.000000,1.000000,0.000000},		//4

	{0.000000,-1.000000,-0.000000},		//5
	{0.000000,-1.000000,-0.000000},		//5
	{0.000000,-1.000000,-0.000000},		//5
	{0.000000,-1.000000,-0.000000},		//5
	
	{0.000000,0.000000,-1.000000},		//0
	{0.000000,0.000000,-1.000000},		//0
	{0.000000,0.000000,-1.000000},		//0
	{0.000000,0.000000,-1.000000},		//0

	{1.000000,0.000000,0.000000},		//1
	{1.000000,0.000000,0.000000},		//1
	{1.000000,0.000000,0.000000},		//1
	{1.000000,0.000000,0.000000},		//1

	{0.000000,0.000000,1.000000},		//2
	{0.000000,0.000000,1.000000},		//2
	{0.000000,0.000000,1.000000},		//2
	{0.000000,0.000000,1.000000},		//2

	{-1.000000,0.000000,0.000000},		//3
	{-1.000000,0.000000,0.000000},		//3
	{-1.000000,0.000000,0.000000},		//3
	{-1.000000,0.000000,0.000000},		//3

	{0.000000,1.000000,0.000000},		//4
	{0.000000,1.000000,0.000000},		//4
	{0.000000,1.000000,0.000000},		//4
	{0.000000,1.000000,0.000000},		//4
};
int boneIndex[] = {
	0,
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,

	0,
	0,
	0,
	0,
	
	1,
	1,
	1,
	1,
	
	1,
	1,
	1,
	1,
	
	1,
	1,
	1,
	1,
	
	1,
	1,
	1,
	1,
	
	1,
	1,
	1,
	1,

};
#define BONE_NUM 2	
CCube::CCube() {
	m_vertexNum = sizeof(vertex)/sizeof(SVector3D);
	m_pVertexArray = new SVector3D[m_vertexNum];
	memcpy(m_pVertexArray,vertex,sizeof(SVector3D)*m_vertexNum);
	m_pNormalArray = new SVector3D[m_vertexNum];
	memcpy(m_pNormalArray,normal,sizeof(SVector3D)*m_vertexNum);

	m_boneIndexArray = new int[ m_vertexNum ];
	memcpy(m_boneIndexArray,boneIndex,sizeof(int)*m_vertexNum);

	
	glGenBuffers(3,m_buffers);
	
	//頂点座標
	glBindBuffer(GL_ARRAY_BUFFER,m_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(SVector3D)*m_vertexNum,m_pVertexArray,GL_STATIC_DRAW);
	//法線
	glBindBuffer(GL_ARRAY_BUFFER,m_buffers[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(SVector3D)*m_vertexNum,m_pNormalArray,GL_STATIC_DRAW);

	//影響ボーンインデックス
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof (int) * m_vertexNum,
								m_boneIndexArray, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER,0);
	m_pMatrixArray = new CMatrix[BONE_NUM];

	m_shader.load("cube.vert",NULL);
};
CCube::~CCube() {
	delete[] m_pVertexArray;
	delete[] m_pNormalArray;
	delete[] m_boneIndexArray;
	delete[] m_pMatrixArray;
}

void CCube::draw(){
	static float r=0,r2=0;
	CMatrix tmp;
	if(::GetKeyState('A') & 0x80) {
		r+=0.01f;
	}
	if(::GetKeyState('Q') & 0x80) {
		r-=0.01f;
	}
	if(::GetKeyState('W') & 0x80) {
		r2+=0.01f;
	}
	if(::GetKeyState('S') & 0x80) {
		r2-=0.01f;
	}
	m_pMatrixArray[0].RotationZ(r);
	tmp.RotationZ(r2);
	m_pMatrixArray[1] = tmp * m_pMatrixArray[0];
	m_shader.enable();
	
	
	//シェーダーへ光源情報を送る
	GLfloat lightVec[4]={0,sin(0.3),cos(0.3),0};
	int lightId = glGetUniformLocation(m_shader.getProgram(),"lightVec");  //ライトの向きを設定
	glUniform3fv(lightId,1,lightVec);
	  
	//シェーダーへ表面色情報を送る
	GLfloat color[]={0.0f,1.0f,1.0f,1.0f};
	int colorId = glGetUniformLocation(m_shader.getProgram(),"baseColor");  //表面の色設定
	glUniform4fv(colorId,1,color);

	
	//シェーダーにボーンマトリックスを送る
	int matrixId = glGetUniformLocation(m_shader.getProgram(),"boneMatrix");  //表面の色設定
	glUniformMatrix4fv(matrixId, BONE_NUM, GL_FALSE, m_pMatrixArray->f);

	//シェーダーへ影響ボーンNoを送る
	int  indexId = glGetAttribLocation(m_shader.getProgram(), "boneIndex");
	glEnableVertexAttribArray(indexId);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[2]);
	glVertexAttribPointer(indexId, 1, GL_INT, GL_FALSE, sizeof(int), NULL);


	glBindBuffer(GL_ARRAY_BUFFER,m_buffers[0]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,0);
	
	glBindBuffer(GL_ARRAY_BUFFER,m_buffers[1]);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT,0,0);

	
	glDrawArrays(GL_QUADS,0,m_vertexNum);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	m_shader.disable();
}