#pragma once

#include <time.h>
#include <windows.h>

class CFPS {
	//ŽžŠÔŒv‘ª—p
	static LARGE_INTEGER freq;
	static LARGE_INTEGER time_buf;
	static int fps;
public:
	static void Init();
	static void Wait();
	static int getFPS(){ return fps;}

};