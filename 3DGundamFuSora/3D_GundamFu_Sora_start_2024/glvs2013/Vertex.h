#pragma once
#include "CVector.h"
#include "windows.h"

//頂点構造体
struct MY_VERTEX
{
	SVector3D		vPos;//頂点位置
	SVector3D		vNorm;//頂点法線
	SVector2D		vTex;//UV座標
	float			bBoneWeight[4];//ボーン　重み
	float			bBoneIndex[4];//ボーン　番号
	MY_VERTEX()
	{
		ZeroMemory(this,sizeof(MY_VERTEX));
	}
};
