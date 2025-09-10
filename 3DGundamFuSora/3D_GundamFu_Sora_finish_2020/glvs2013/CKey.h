#pragma once
#include "windows.h"

#define PAD_MAX 5

class CKey {
public:
	enum E_STATE{
		ePush,
		eHold,
		ePull,
		eFree,
		eStateMax,
	};
	enum E_BUTTON{
		eUp = 0,
		eDown,
		eLeft,
		eRight,
		eButton1,
		eButton2,
		eButton3,
		eButton4,
		eButton5,
		eButton6,
		eButton7,
		eMax = 16
	};
	static char	m_key_code[PAD_MAX][eMax];
	static unsigned long m_key_old[PAD_MAX];
	static unsigned long m_key_state[PAD_MAX][eStateMax];
public:
	static bool getState(int no,E_STATE state,E_BUTTON button){
		return (m_key_state[no][state] & (1<<button));
	}
	static void setState(int no,E_STATE state,E_BUTTON button){
		m_key_state[no][state] |= (1<<button);
	}
	static void setButton(int no,E_BUTTON button,char code){
		m_key_code[no][button] = code;
	}
	static void upDate();
	

};