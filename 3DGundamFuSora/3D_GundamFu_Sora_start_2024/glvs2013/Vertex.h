#pragma once
#include "CVector.h"
#include "windows.h"

//���_�\����
struct MY_VERTEX
{
	SVector3D		vPos;//���_�ʒu
	SVector3D		vNorm;//���_�@��
	SVector2D		vTex;//UV���W
	float			bBoneWeight[4];//�{�[���@�d��
	float			bBoneIndex[4];//�{�[���@�ԍ�
	MY_VERTEX()
	{
		ZeroMemory(this,sizeof(MY_VERTEX));
	}
};
