#pragma once
#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
class CSoundBase {
	int	m_layer;
	int m_current;
	IXAudio2SourceVoice** m_pSourceVoice;
	BYTE* m_pWavBuffer;//�g�`�f�[�^�i�t�H�[�}�b�g�����܂܂Ȃ��A�����ɔg�`�f�[�^�̂݁j
	DWORD m_dwWavSize;//�g�`�f�[�^�̃T�C�Y

public:
	bool load(char *path,int layer);
	int play(bool boLoop=false);
	void stop(int layer=-1);
	void pause(int layer=-1);
};
class CSound {
	CSoundBase *m_core;
	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;
	static CSound *m_sound;
	friend class CSoundBase;
public:
	static CSound* getSound(){return m_sound;}
	static bool create(int max);
	int play(int no,bool boLoop = false);
	void stop(int no,int layer = -1);
	void pause(int no,int layer = -1);
	bool load(int no,char* path,int layer);
};

