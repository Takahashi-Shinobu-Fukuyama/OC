#include "CKey.h"

char		CKey::m_key_code[PAD_MAX][CKey::eMax]={0};
unsigned long CKey::m_key_old[PAD_MAX]={0};
unsigned long CKey::m_key_state[PAD_MAX][CKey::eStateMax]={0};

void CKey::upDate(){
	for(int i=0;i<PAD_MAX;i++) {
		unsigned long key = 0;
		unsigned long state = 0;
		for(int j=0;j<eMax;j++) {
			if(::GetKeyState(m_key_code[i][j]) & 0x80) {
				key |= 1<<j;
			}
		}
		m_key_state[i][ePush] = (~m_key_old[i]) & key;
		m_key_state[i][eHold] = m_key_old[i] & key;
		m_key_state[i][ePull] = m_key_old[i] & (~key);
		m_key_state[i][eFree] = (~m_key_old[i]) & (~key);
		m_key_old[i] = key;
	}
}
