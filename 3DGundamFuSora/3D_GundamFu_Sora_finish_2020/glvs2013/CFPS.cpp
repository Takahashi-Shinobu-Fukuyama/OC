#include "CFPS.h"

//�P�b�Ԃ̃J�E���g��
LARGE_INTEGER CFPS::freq;
//�O�t���[���̃J�E���g��
LARGE_INTEGER CFPS::time_buf;
int CFPS::fps;

void CFPS::Init(){
	//���Ԍv���p
	//�V�X�e���̂P�b�Ԃ̃J�E���g���i���g���j���擾
	QueryPerformanceFrequency(&freq);
	//���݂̃V�X�e���̃J�E���g�����擾
	QueryPerformanceCounter(&time_buf);
}

void CFPS::Wait() {
	//freq.QuadPart�@	1�b������̃J�E���g���i�Œ�j
	//time.QuadPart�@	���̃J�E���g��
	//time_buf.QuadPart�@�O��̃J�E���g��

	//��L��3�̏�񂩂�A�����őO��Ăяo�������1/60�b�o�܂őҋ@����

	LARGE_INTEGER time;
	//�O�t���[���ƍ��̃J�E���g�̍���1/60�b�����Ȃ烋�[�v
	do{
		//���݂̃V�X�e���̃J�E���g�����擾
		QueryPerformanceCounter(&time);

		//�@���̃J�E���g�@�O��̃J�E���g�@ < 1�b������̃J�E���g����60�Ŋ���(1/60�b������̃J�E���g��)
	}while(time.QuadPart-time_buf.QuadPart < freq.QuadPart /60);

	fps = freq.QuadPart/(time.QuadPart-time_buf.QuadPart);
	time_buf = time;

}