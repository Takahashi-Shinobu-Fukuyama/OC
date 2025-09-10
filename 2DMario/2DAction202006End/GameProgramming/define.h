#include "windows.h"
//マクロ
#define WINDOW_PIXEL_H	640	//ウィンドウ解像度：幅
#define WINDOW_PIXEL_V	480	//ウィンドウ解像度：高さ
#define XMAX  640.0f			//X座標の上限
#define YMAX  WINDOW_PIXEL_V * XMAX / WINDOW_PIXEL_H	//Y座標の上限
#define KEY(c)  GetKeyState(c) & 0x8000	//キーボードが押されているか判定するマクロ関数
#define TIPX	36.f
#define TIPY	36.f
#define PLAYER_SPEED_X	5	//	5
#define PLAYER_SPEED_Y	2	//	2
#define PLAYER_SPEED_J	40	//	40
#define ENEMY_SPEED_X	3	//	3
#define ENEMY_SPEED_Y	2	//	2

