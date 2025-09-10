#include "CSound.h"

#pragma comment(lib,"winmm")

bool CSoundBase::load(char* path,int layer)
{
	m_layer = layer;
	m_current = 0;
	m_pSourceVoice = new IXAudio2SourceVoice*[layer];
		HMMIO hMmio=NULL;//Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API)
		DWORD dwWavSize=0;//WAV�t�@�C�����@WAV�f�[�^�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���̂ŁA�قڃt�@�C���T�C�Y�Ɠ���j
		WAVEFORMATEX* pwfex;//WAV�̃t�H�[�}�b�g ��j16�r�b�g�A44100Hz�A�X�e���I�Ȃ�
		MMCKINFO ckInfo;//�@�`�����N���
		MMCKINFO riffckInfo;// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
		PCMWAVEFORMAT pcmWaveForm;
		//WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
		hMmio=mmioOpenA(path, NULL, MMIO_ALLOCBUF | MMIO_READ);
		//�t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
		mmioDescend( hMmio, &riffckInfo, NULL, 0 );
		// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
		ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend( hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );
		//�t�H�[�}�b�g��ǂݍ���
		mmioRead( hMmio, (HPSTR) &pcmWaveForm,sizeof(pcmWaveForm));
		pwfex = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
		memcpy( pwfex, &pcmWaveForm, sizeof(pcmWaveForm) );
		pwfex->cbSize = 0;	
		mmioAscend( hMmio, &ckInfo, 0 );
		// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���	
		ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend( hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );//�f�[�^�`�����N�ɃZ�b�g
		dwWavSize = ckInfo.cksize;
		m_pWavBuffer=new BYTE[ dwWavSize ];
		DWORD dwOffset=ckInfo.dwDataOffset;
		mmioRead( hMmio, (HPSTR)m_pWavBuffer,dwWavSize);
		m_dwWavSize=dwWavSize;

	for(int i=0;i<m_layer;i++) {
		//�\�[�X�{�C�X�Ƀf�[�^���l�ߍ���	
		if(FAILED(CSound::getSound()->m_pXAudio2->CreateSourceVoice(&m_pSourceVoice[i],pwfex)))
		{
			MessageBox(0,"�\�[�X�{�C�X�쐬���s",0,MB_OK);
			return FALSE;
		}
	}
	return TRUE;
}

int CSoundBase::play(bool boLoop ){
	XAUDIO2_BUFFER buffer={0};
	buffer.pAudioData=m_pWavBuffer;
	buffer.Flags=XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes=m_dwWavSize;
	if(boLoop)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	if(FAILED(m_pSourceVoice[m_current]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0,"�\�[�X�{�C�X�ɃT�u�~�b�g���s",0,MB_OK);
		return -1;
	}
	int c=m_current;
	m_pSourceVoice[m_current]->Start(0,XAUDIO2_COMMIT_NOW);
	printf("play %d\n",m_current);
	m_current = (m_current+1) % m_layer;

	return c;

}
void CSoundBase::stop(int layer){
	if(layer == -1) {
		for(int i=0;i<m_layer;i++) {
			m_pSourceVoice[i]->Stop(0,XAUDIO2_COMMIT_NOW);
			m_pSourceVoice[i]->FlushSourceBuffers();
		}
	} else {
		m_pSourceVoice[layer]->Stop(0,XAUDIO2_COMMIT_NOW);
		m_pSourceVoice[layer]->FlushSourceBuffers();
	}
}
void CSoundBase::pause(int layer){
	if(layer == -1) {
		for(int i=0;i<m_layer;i++) {
			m_pSourceVoice[i]->Stop(0,XAUDIO2_COMMIT_NOW);
		}
	} else {
		m_pSourceVoice[layer]->Stop(0,XAUDIO2_COMMIT_NOW);
	}
}

CSound*	CSound::m_sound = NULL;

bool CSound::create(int max){
	m_sound = new CSound();
	CoInitializeEx(NULL,COINIT_MULTITHREADED);	

	/*
	if(FAILED(XAudio2Create(&m_sound->m_pXAudio2,XAUDIO2_DEBUG_ENGINE)))
	{
		CoUninitialize();
		return FALSE;
	}
	*/
	if( FAILED(m_sound->m_pXAudio2->CreateMasteringVoice(&m_sound->m_pMasteringVoice)))
	{
		CoUninitialize();	
		return FALSE;
	}
	m_sound->m_core = new CSoundBase[max];
	return TRUE;

}
int CSound::play(int no,bool boLoop){
	return m_core[no].play(boLoop);
}
void CSound::stop(int no,int layer){
	return m_core[no].stop(layer);
}
void CSound::pause(int no,int layer){
	return m_core[no].pause(layer);
}
bool CSound::load(int no,char* path,int layer){
	return m_core[no].load(path,layer);
}