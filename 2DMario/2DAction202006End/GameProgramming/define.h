#include "windows.h"
//�}�N��
#define WINDOW_PIXEL_H	640	//�E�B���h�E�𑜓x�F��
#define WINDOW_PIXEL_V	480	//�E�B���h�E�𑜓x�F����
#define XMAX  640.0f			//X���W�̏��
#define YMAX  WINDOW_PIXEL_V * XMAX / WINDOW_PIXEL_H	//Y���W�̏��
#define KEY(c)  GetKeyState(c) & 0x8000	//�L�[�{�[�h��������Ă��邩���肷��}�N���֐�
#define TIPX	36.f
#define TIPY	36.f
#define PLAYER_SPEED_X	5	//	5
#define PLAYER_SPEED_Y	2	//	2
#define PLAYER_SPEED_J	40	//	40
#define ENEMY_SPEED_X	3	//	3
#define ENEMY_SPEED_Y	2	//	2

