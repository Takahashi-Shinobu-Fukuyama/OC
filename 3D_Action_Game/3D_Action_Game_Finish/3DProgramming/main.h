#ifndef MAIN_H
#define MAIN_H

#include "CMatrix44.h"

#define ZNEAR 1.0
#define ZFAR 1000.0
#define WINDOW_W 800
#define WINDOW_H 600
/*���C�g�ݒ�*/
#define LIGHT_SET_DIR 0,-1,1 //�g�U��
#define LIGHT_SET_AMB 0.9f, 0.9f, 0.9f //����
#define LIGHT_SET_COL 1.0f, 1.0f, 1.0f //�J���[
#define LIGHT_POS  1.0f, 1.0f, 1.0f, 0.0f //�|�W�V����

extern CMatrix44 GMatrixViewport;
extern CMatrix44 GMatrixProjection;
extern CMatrix44 GMatrixModelView;

extern CVector3 WorldToViewportPoint(const CVector3 &v);

#endif
