#include "define.h"
#include "glut.h"
#define _USE_MATH_DEFINES	//���w���C�u������M_PI��L���ɂ���
#include <math.h>	//���w���C�u�����̃C���N���[�h
#include <stdio.h>

class CGame {
public:
	static float rangeXS;
	static float rangeXE;
};

float CGame::rangeXS = 0.0f;
float CGame::rangeXE = 0.0f;


class CKey {
	static bool flg[256];
public:
	static bool push(char k);
	static bool once(char k);

};
bool CKey::flg[256];

bool CKey::push(char k) {
	return KEY(k);
}

bool CKey::once(char k) {
	if (KEY(k)) {
		if (flg[k]) {
			return false;
		}
		else {
			flg[k] = true;
			return true;
		}
	}
	else {
		flg[k] = false;
	}
	return false;
}

/* 2�����̃x�N�g���N���X���쐬���� */
class CVector2 {
public:
	float x;	//X���ւ̑傫��
	float y;	//Y���ւ̑傫��
	float d;	//�_�~�[

	CVector2() 
		:
		x(0.0), y(0.0f), d(0.0f) {};	//�f�t�H���g�R���X�g���N�^
	CVector2(float x, float y) 
		:
		x(x), y(y), d(0.0f) {};	//�R���X�g���N�^
	CVector2(float x, float y, float d) 
		:
		x(x), y(y), d(d) {};	//�R���X�g���N�^

	/*�@+=���Z�q�̃I�[�o�[���C�h
	CVector2 += CVector2�@�̐錾
	*/
	void operator+=(const CVector2 &v) {
		x += v.x; y += v.y;
	}
	/*�@*=���Z�q�̃I�[�o�[���C�h
	CVector2 *= CVector2�@�̐錾
	*/
	void operator*=(const CVector2 &v) {
		x *= v.x; y *= v.y;
	}
	/*�@*=���Z�q�̃I�[�o�[���C�h
	CVector2 *= float�@�̐錾
	*/
	void operator*=(float f) {
		x *= f; y *= f;
	}
	/*�@*���Z�q�̃I�[�o�[���C�h
	CVector2 * float�@�̐錾
	*/
	const CVector2 operator*(float f) const {
		return CVector2(x * f, y * f);
	}

	const CVector2 operator+(const CVector2 &v) const {
		return CVector2(x + v.x, y + v.y);
	}

	float dot(const CVector2 &v) {
		return x * v.x + y * v.y;
	}

	void normalize() {
		float len = length();
		x /= len;
		y /= len;
	}

	float length() {
		return sqrt(x*x + y*y);
	}

};

/*
TGA�t�@�C���̃w�b�_�t�H�[�}�b�g
*/
struct STgaheader {
	unsigned char	dummy1[12];
	unsigned short	width;		//��
	unsigned short	height;	//����
	unsigned char	depth;	//�r�b�g�̐[��
	unsigned char	dummy2;
};
/*
�e�N�X�`���[�N���X
*/
class CTexture {
public:
	//�e�N�X�`��ID
	GLuint id;
	//TGA�t�@�C���̃w�b�_���
	STgaheader header;
	/*
	�f�t�H���g�R���X�g���N�^
	*/
	CTexture() : id(0) {}
	/*
	�f�X�g���N�^�i���̃C���X�^���X���j�������Ƃ��Ɏ��s�����j
	*/
	~CTexture() {
		//�e�N�X�`���f�[�^������΍폜����
		if (id != 0) {
			//�e�N�X�`���f�[�^�̍폜
			glDeleteTextures(1, &id);
			id = 0;
		}
	}

	/*
	�摜�t�@�C���̓ǂݍ��݁i�e�N�X�`���t�@�C���̓ǂݍ��݁j
	*/
	void load(const char* filename) {
		//�摜�f�[�^
		unsigned char* data;
		//�t�@�C���|�C���^�̍쐬
		FILE *fp;
		//�t�@�C���I�[�v��
		fp = fopen(filename, "rb");
		//�G���[�̂Ƃ��̓��^�[��
		if (!fp) {
			printf("file not found:%s\n", filename);
			return;
		}
		//�w�b�_����ǂݍ���
		fread(&header, sizeof(header), 1, fp);
		//�摜�f�[�^�̃o�C�g�����v�Z����
		int length = header.width * header.height * header.depth / 8;
		//�摜�f�[�^�̊i�[�̈���m��
		data = new unsigned char[length];
		//�摜�f�[�^���w�b�_�̑�������ǂݍ���
		fread(data, length, 1, fp);
		//�摜�t�@�C���̃N���[�Y
		fclose(fp);

		//�e�N�X�`���f�[�^�̍쐬
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (header.depth == 32)
			//�A���t�@�L��̃e�N�X�`���쐬
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, header.width,
			header.height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
		else
			//�A���t�@�����̃e�N�X�`���쐬
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, header.width,
			header.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
		//�i�[�̈�����
		delete[] data;
	}

};
/* �����`�N���X */
class  CRectangle {
public:
	//���W
	CVector2 v1L, v2L, v3L, v4L;
	CVector2 v1W, v2W, v3W, v4W;

	//�F
	float r, g, b, a;	//r:�� g:�� b:�� a:�A���t�@�i�����x�j

	//�e�N�X�`��
	CTexture	*ptexture;	//�e�N�X�`���̃|�C���^
	float		uL, uR, vT, vB;	//�e�N�X�`���}�b�s���O�̒l

	CRectangle() 
		:
		r(0.0f), g(0.0f), b(0.0f), a(0.0f), uL(0.0f), uR(0.0f), vT(0.0f), vB(0.0f),ptexture(0) {}

	CRectangle(float left, float top, float right, float bottom,
		float r, float g, float b, float a)
		:
		v1L(left, top), v2L(left, bottom), v3L(right, bottom), v4L(right, top),
		r(r), g(g), b(b), a(a), uL(0.0f), vT(0.0f), uR(0.0f), vB(0.0f), ptexture(0)
	{}

	CRectangle(float left, float top, float right, float bottom,
		float r, float g, float b, float a,
		CTexture *ptexture, float ul, float ur, float vt, float vb) 
		:
		v1L(left, top), v2L(left, bottom), v3L(right, bottom), v4L(right, top),
		r(r), g(g), b(b), a(a)
	{
		setTexture(ptexture, ul, vt, ur, vb);
	}

	void update(CVector2 position) {
		v1W = v1L + position;
		v2W = v2L + position;
		v3W = v3L + position;
		v4W = v4L + position;
	}

	//�`�悷��
	void render() {
		//�A���t�@�u�����h��L���ɂ���
		glEnable(GL_BLEND);
		//�u�����h���@���w��
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//�e�N�X�`���̗L��
		if (ptexture == 0) {
			//�`��F���w��
			glColor4f(r, g, b, a);
			//�e�N�X�`���̖����Ƃ�
			glBegin(GL_QUADS);
			{
				glVertex2f(v1W.x, v1W.y);
				glVertex2f(v2W.x, v2W.y);
				glVertex2f(v3W.x, v3W.y);
				glVertex2f(v4W.x, v4W.y);
			}
			glEnd();
		}
		else {
			//�e�N�X�`�����L��Ƃ�
			//�e�N�X�`����L���ɂ���
			glEnable(GL_TEXTURE_2D);
			//�e�N�X�`�����w��
			glBindTexture(GL_TEXTURE_2D, ptexture->id);

			//�`��F���w��
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glBegin(GL_QUADS);
			{
				glTexCoord2f(uL, vT);
				glVertex2f(v1W.x, v1W.y);

				glTexCoord2f(uL, vB);
				glVertex2f(v2W.x, v2W.y);

				glTexCoord2f(uR, vB);
				glVertex2f(v3W.x, v3W.y);

				glTexCoord2f(uR, vT);
				glVertex2f(v4W.x, v4W.y);
			}
			glEnd();

			//�e�N�X�`�������
			glBindTexture(GL_TEXTURE_2D, 0);
			//�e�N�X�`���𖳌�
			glDisable(GL_TEXTURE_2D);
		}
		//�A���t�@�u�����h�𖳌�
		glDisable(GL_BLEND);
	}

	/*
	�e�N�X�`�����̐ݒ�
	*t		CTexture�̃C���X�^���X�̃|�C���^
	left	�摜�̍���[�����_(0,0)�Ƃ��A�\��t�������̈�̍��̍��W�l
	top		�摜�̍���[�����_(0,0)�Ƃ��A�\��t�������̈�̏�̍��W�l
	right	�摜�̍���[�����_(0,0)�Ƃ��A�\��t�������̈�̉E�̍��W�l
	bottom	�摜�̍���[�����_(0,0)�Ƃ��A�\��t�������̈�̉��̍��W�l
	*/
	void setTexture(CTexture *t, int left, int top, int right, int bottom) {
		ptexture = t;
		uL = (float)(left - 1) / (ptexture->header.width);
		vT = (float)(ptexture->header.height - top) / (ptexture->header.height);
		vB = (float)(ptexture->header.height - bottom) / (ptexture->header.height);
		uR = (float)(right - 1) / (ptexture->header.width);
	}

	void setColor(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

};

enum EObjectType {
	Player,
	GrdBlock,
	Kuribo,
	Nokonoko,
	Hatena,
	Akakinoko,
	EnemyBlock,
	Coin,
};

enum EStatus {
	Active,
	Active2,
	Active3,
	Stop
};

class CBase {
public:
	CRectangle rect;
	CVector2 position;
	CVector2 rotation;
	CVector2 forward;
	CVector2 speed;
	bool enabled = true;
	bool collisionFlg = true;
	bool jumpFlg = true;
	EObjectType objectType;
	EStatus status = Active;
	bool turnx = true;

	CBase() {}

	CBase(float left, float top, float right, float bottom,
		float r, float g, float b, float a,
		float px, float py) 
		:
		rect(left, top, right, bottom, r, g, b, a),
		position(px, py),
		collisionFlg(true),
		jumpFlg(true),
		enabled(true)
	{}

	void setSpeed(float x, float y) {
		speed.x = x;
		speed.y = y;
	}

	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void setPosition(int i, int hsize) {
		position.x = i / hsize * TIPX * 2 ;
		position.y = i % hsize * TIPY * 2 ;
	}

	void setTexture(CTexture *p, int ul, int vt, int ur, int vb) {
		rect.setTexture(p, ul, vt, ur, vb);
	}

	void setObjectType(EObjectType ot) {
		objectType = ot;
	}

	void setColor(float r, float g, float b, float a) {
		rect.setColor(r, g, b, a);
	}

	void setTexture(CTexture *t) {
		rect.ptexture = t;
	}

	void setuv(int uL, int vT, int uR, int vB) {
		setTexture(rect.ptexture, uL, vT, uR, vB);
	}

	virtual void update() {
		rect.update(position);
		if (position.y < -YMAX){
			enabled = false;
		}
	}

	virtual void render() {
		rect.render();
	}

	virtual void onCollision(const CBase &b) {}

	void disabled() {
		enabled = false;
	}
};

class CCollision {
public:
	static bool CCollision::collision(CRectangle &r1, CRectangle &r2);
	/*
	�l�p�`r1�Ǝl�p�`r2��fwd�����ړ����d�Ȃ����ꍇ�Artn�����߂��Ώd�Ȃ�Ȃ�
	*/
	static bool collision(const CRectangle &r1, const CRectangle &r2, const CVector2 &fwd, CVector2 *rtn);
	//�l�p�`�Ɛ���
	static bool collision(const CVector2 &v1, const CVector2 &v2, const CVector2 &v3, const CVector2 &v4, const CVector2 &s, const CVector2 &e, CVector2 *t);
	//����V11V12�Ɛ���V21V22�̌�������
	static bool collision(const CVector2 &v11, const CVector2 &v12, const CVector2 &v21, const CVector2 &v22, CVector2 *rt);

};

bool CCollision::collision(CRectangle &r1, CRectangle &r2){
	if (r1.v1W.x < r2.v3W.x && r2.v1W.x < r1.v3W.x && r1.v1W.y > r2.v3W.y && r1.v3W.y < r2.v1W.y){
		return true;
	}
	return false;
}
/*
�l�p�`r1�Ǝl�p�`r2��fwd�����ړ����d�Ȃ����ꍇ�Artn�����߂��Ώd�Ȃ�Ȃ�
*/
bool CCollision::collision(const CRectangle &r1, const CRectangle &r2, const CVector2 &fwd, CVector2 *rtn) {
	if ((fwd.x < 0 || fwd.y > 0) && collision(r1.v1W, r1.v2W, r1.v3W, r1.v4W, r2.v1W, r2.v1W + fwd * -1, rtn)) {
		return true;
	}
	if ((fwd.x < 0 || fwd.y < 0) && collision(r1.v1W, r1.v2W, r1.v3W, r1.v4W, r2.v2W, r2.v2W + fwd * -1, rtn)) {
		return true;
	}
	if ((fwd.x > 0 || fwd.y < 0) && collision(r1.v1W, r1.v2W, r1.v3W, r1.v4W, r2.v3W, r2.v3W + fwd * -1, rtn)) {
		return true;
	}
	if ((fwd.x > 0 || fwd.y > 0) && collision(r1.v1W, r1.v2W, r1.v3W, r1.v4W, r2.v4W, r2.v4W + fwd * -1, rtn)) {
		return true;
	}

	//
	if ((fwd.x > 0 || fwd.y < 0) && collision(r2.v1W, r2.v2W, r2.v3W, r2.v4W, r1.v1W, r1.v1W + fwd, rtn)) {
		rtn->x = rtn->x * -1.0f;
		rtn->y = rtn->y * -1.0f;
		return true;
	}
	if ((fwd.x > 0 || fwd.y > 0) && collision(r2.v1W, r2.v2W, r2.v3W, r2.v4W, r1.v2W, r1.v2W + fwd, rtn)) {
		rtn->x = rtn->x * -1.0f;
		rtn->y = rtn->y * -1.0f;
		return true;
	}
	if ((fwd.x < 0 || fwd.y > 0) && collision(r2.v1W, r2.v2W, r2.v3W, r2.v4W, r1.v3W, r1.v3W + fwd, rtn)) {
		rtn->x = rtn->x * -1.0f;
		rtn->y = rtn->y * -1.0f;
		return true;
	}
	if ((fwd.x < 0 || fwd.y < 0) && collision(r2.v1W, r2.v2W, r2.v3W, r2.v4W, r1.v4W, r1.v4W + fwd, rtn)) {
		rtn->x = rtn->x * -1.0f;
		rtn->y = rtn->y * -1.0f;
		return true;
	}
	return false;
}

//�l�p�`�Ɛ���
bool CCollision::collision(const CVector2 &v1, const CVector2 &v2, const CVector2 &v3, const CVector2 &v4, const CVector2 &s, const CVector2 &e, CVector2 *t) {
	//V12�ƌ������邩����
	if (collision(v1, v2, s, e, t)) {
		return true;
	}
	//V23�ƌ������邩����
	if (collision(v2, v3, s, e, t)) {
		return true;
	}
	//V34�ƌ������邩����
	if (collision(v3, v4, s, e, t)) {
		return true;
	}
	//V41�ƌ������邩����
	if (collision(v4, v1, s, e, t)) {
		return true;
	}
	//�������Ă��Ȃ�
	return false;
}

//����V11V12�Ɛ���V21V22�̌�������
bool CCollision::collision(const CVector2 &v11, const CVector2 &v12, const CVector2 &v21, const CVector2 &v22, CVector2 *rt) {
	CVector2 v1112 = v12 + v11 * -1;
	CVector2 n1112;
	n1112.x = -v1112.y;
	n1112.y = v1112.x;
	n1112.normalize();
	float dots = n1112.dot(v21 + v11 * -1);
	float dote = n1112.dot(v22 + v11 * -1);
	//�v���X��������������Ă��Ȃ�
	if (dots * dote > 0) {
		return false;
	}
	else {
		//�n�_���O�͌����Ȃ��Ƃ���
		if (dots < 0) {
			return false;
		}
		//����0���Ɛ���B����͏Փ˂��Ă��Ȃ��B
		if (dots == 0.0f && dote == 0.0f) {
			return false;
		}
		dots = -dots / (-dots + dote);
	}
	CVector2 v5 = v22 + v21 * -1;
	v5 = v5 * dots;
	v5 = v21 + v5;
	//��_v5��v1112�ォ
	float length = v1112.length();
	v1112.normalize();
	dote = v1112.dot(v5 + v11*-1);
	//����͌������Ă��Ȃ�
	if (dote <= 0 || length <= dote){
		return false;
	}
	//�����m��i�߂��ʌv�Z�j
	n1112 = n1112 * n1112.dot(v5 + v21*-1);
	rt->x = n1112.x;
	rt->y = n1112.y;

	return true;
}

class CPlayer : public CBase {
public:
	int cnt = 0;
	CPlayer()
		:
		CBase(-(TIPX-2.0f), TIPY - 2.0f, TIPX - 2.0f, -(TIPY - 2.0f), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)
	{
		setObjectType(Player);
		setSpeed(PLAYER_SPEED_X, 0.0f);
	}

	void update() {
		switch (status){
		case Stop:
			if (turnx) {
				forward.x = 0;
				forward.y = 0;
				turnx = false;
			}
			else {
				collisionFlg = false;
				setuv(162, 32, 177, 48);
				speed.y -= PLAYER_SPEED_Y;
				if (speed.y < -PLAYER_SPEED_Y) { // during down not jump
					jumpFlg = false;
				}
				else if (speed.y < -TIPY) {
					speed.y = -TIPY;
				}
				forward.y = speed.y;
				forward.x = 0.0f;
				turnx = true;
			}
			break;
		case Active3:	// Between Big and Normal Size
			cnt++;
			if (cnt > 100) {
				status = Active;
				cnt = 0;
			}
		case Active:	// Normal Size
			if (speed.y > 0) {
				if (speed.x > 0) {
					setuv(145, 32, 160, 48);
				}
				else {
					setuv(160, 32, 145, 48);
				}
			}
			else {
				if (speed.x > 0) {
					setuv(97, 32, 112, 48);
				}
				else {
					setuv(112, 32, 97, 48);
				}
			}
			if (turnx) {	// move x
				if (CKey::push('D')) {
					speed.x = PLAYER_SPEED_X;
					forward.x = speed.x;
					forward.y = 0.0f;
					if (jumpFlg) { // move ground
						if (int(position.x) % (PLAYER_SPEED_X * 20) < PLAYER_SPEED_X * 10) {
							setuv(81, 32, 96, 48);
						}
						else {
							setuv(97, 32, 112, 48);
						}
					}
				}
				else if (CKey::push('A')) {
					speed.x = -PLAYER_SPEED_X;
					forward.x = speed.x;
					forward.y = 0.0f;
					if (jumpFlg) { // move ground
						if (int(position.x) % (PLAYER_SPEED_X * 20) < PLAYER_SPEED_X * 10) {
							setuv(96, 32, 81, 48);
						}
						else {
							setuv(112, 32, 97, 48);
						}
					}
				}
				else{
					forward.x = 0.0f;
					forward.y = 0.0f;
				}
				turnx = false;
			}
			else {	// move y
				if (jumpFlg && CKey::once('J')) {	// jump start
					speed.y = PLAYER_SPEED_J;
					jumpFlg = false;
				}
				speed.y -= PLAYER_SPEED_Y;
				if (speed.y < -PLAYER_SPEED_Y) { // during down not jump
					jumpFlg = false;
				}
				else if (speed.y < -TIPY) {
					speed.y = -TIPY;
				}
				forward.y = speed.y;
				forward.x = 0.0f;
				turnx = true;
			}
			break;
		case Active2:	// Big Size
			if (speed.y > 0) { // jump up
				if (speed.x > 0) {
					setuv(145, 1, 160, 32);
				}
				else {
					setuv(160, 1, 145, 32);
				}
			}
			else {	// jump down
				if (speed.x > 0) {
					setuv(97, 1, 112, 32);
				}
				else {
					setuv(112, 1, 97, 32);
				}
			}
			if (turnx) {
				if (CKey::push('D')) {
					speed.x = PLAYER_SPEED_X * 1.5f;
					forward.x = speed.x;
					forward.y = 0.0f;
					if (jumpFlg) {	// jump able ground stand
						if (int(position.x) % (PLAYER_SPEED_X * 20) < PLAYER_SPEED_X * 10) {
							setuv(81, 1, 96, 32);
						}
						else {
							setuv(97, 1, 112, 32);
						}
					}
				}
				else if (CKey::push('A')) {
					speed.x = -PLAYER_SPEED_X * 1.5f;
					forward.x = speed.x;
					forward.y = 0.0f;
					if (jumpFlg) {
						if (int(position.x) % (PLAYER_SPEED_X * 20) < PLAYER_SPEED_X * 10) {
							setuv(96, 1, 81, 32);
						}
						else {
							setuv(112, 1, 97, 32);
						}
					}
				}
				else{
					forward.x = 0.0f;
					forward.y = 0.0f;
				}
				turnx = false;
			}
			else {
				if (jumpFlg && CKey::once('J')) {
					speed.y = PLAYER_SPEED_J + 5;
					jumpFlg = false;
				}
				speed.y -= PLAYER_SPEED_Y;
				if (speed.y < -PLAYER_SPEED_Y) {
					jumpFlg = false;
				}
				else if (speed.y < -TIPY) {
					speed.y = -TIPY;
				}
				forward.y = speed.y;
				forward.x = 0.0f;
				turnx = true;
			}
			break;
		}
		position += forward;
		CBase::update();
		CGame::rangeXS = -XMAX + position.x;
		CGame::rangeXE = XMAX + position.x;
	}

	void onCollision(const CBase &b) {
		CVector2 v;
		switch (b.objectType) {
		case EnemyBlock:
			break;
		case Akakinoko:
			if (b.status == Stop) {	// moved kinoko hit
				status = Active2;
				setuv(80, 0, 96, 32);
				rect.v1L.y = TIPY * 2;
				rect.v4L.y = TIPY * 2;
			}
			break;
		case Kuribo:
		case Nokonoko:
			if (b.status != Stop) {
				if (turnx) {
					if (CCollision::collision(b.rect, rect, forward, &v)) {
						if (forward.y >= 0.0f || v.y <= 0.0f) {
							if (jumpFlg == true) {
								if (status == Active2 || status == Active3) {
									status = Active3;
									rect.v1L.y = TIPY - 2.0f;
									rect.v4L.y = TIPY - 2.0f;
								}
								else {
									speed.y = PLAYER_SPEED_J;
									status = Stop;
								}
							}
							else {
								if (speed.y > 0.0f) {
									if (status == Active2 || status == Active3) {
										status = Active3;
										rect.v1L.y = TIPY - 2.0f;
										rect.v4L.y = TIPY - 2.0f;
									}
									else {
										speed.y = PLAYER_SPEED_J;
										status = Stop;
									}
								}
							}
						}
					}
				}
				else {
					if (jumpFlg == true) {
						if (status == Active2 || status == Active3) {
							status = Active3;
							rect.v1L.y = TIPY - 2.0f;
							rect.v4L.y = TIPY - 2.0f;
						}
						else {
							speed.y = PLAYER_SPEED_J;
							status = Stop;
						}
					}
					else {
						if (speed.y > 0.0f) {
							if (status == Active2 || status == Active3) {
								status = Active3;
								rect.v1L.y = TIPY - 2.0f;
								rect.v4L.y = TIPY - 2.0f;
							}
							else {
								speed.y = PLAYER_SPEED_J;
								status = Stop;
							}
						}
					}
				}
			}
		case GrdBlock:
			if (CCollision::collision(b.rect, rect, forward, &v)) {
				position += v;
				CBase::update();
				CGame::rangeXS = -XMAX + position.x;
				CGame::rangeXE = XMAX + position.x;
				if (turnx) {
					speed.y = 0.0f;
					jumpFlg = true;	//�W�����v�\
				}
			}
		}
	}
};

class CKuribo : public CBase {
public:
	int descnt = 0;

	CKuribo()
		:CBase(-(TIPX - 0.2f), TIPY - 0.2f, TIPX - 0.2f, -(TIPY - 0.2f), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)
	{
		setObjectType(Kuribo);
		setSpeed(-ENEMY_SPEED_X, 0.0f);
	}

	void update() {
		switch (status) {
		case Active:
			if (turnx) {
				forward.x = speed.x;
				forward.y = 0.0f;
				if (int(position.x) % (ENEMY_SPEED_X * 20) < (ENEMY_SPEED_X * 10)) {
					setuv(1, 16, 17, 32);
				}
				else {
					setuv(17, 16, 33, 32);
				}
				turnx = false;
			}
			else {
				speed.y -= ENEMY_SPEED_Y;
				if (speed.y < -TIPY) {
					speed.y = -TIPY;
				}
				forward.y = speed.y;
				forward.x = 0.0f;
				turnx = true;
			}

			position += forward;
			CBase::update();
			break;
		default:
			if (descnt++ > 60)
				enabled = false;
		}
	}
	

	void onCollision(const CBase &b) {
		CVector2 v;
		switch (b.objectType) {
		case Player:
				if (b.jumpFlg == false && b.speed.y < 0.0f) {
					speed.x = 0.0f;
					speed.y = 0.0f;
					rect.v1L.y = 0.0f;
					rect.v4L.y = 0.0f;
					setuv(33, 24, 48, 32);
					CBase::update();
					status = Stop;
					descnt = 0;
				}
			break;
		case Nokonoko:
			if (b.status == Active3) {
				collisionFlg = false;
				speed.x = b.speed.x;
				speed.y = ENEMY_SPEED_Y * 15;
			}
			break;
		case EnemyBlock:
		case GrdBlock:
			if (CCollision::collision(b.rect, rect, forward, &v)) {
				position += v;
				CBase::update();
				if (turnx) {
					speed.y = 0.0f;
				}
				else {
					speed.x = -speed.x;
				}
			}
		}

	}

};

class CNokonoko : public CBase {
public:
	int descnt = 0;

	CNokonoko()
		:CBase(-(TIPX - 0.2f) , TIPY - 0.2f + TIPY / 2, TIPX - 0.2f, -(TIPY - 0.2f), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)
	{
		setObjectType(Nokonoko);
		setSpeed(-ENEMY_SPEED_X, 0.0f);
	}

	void update() {
		switch (status) {
		case Active:	// Normal
			if (turnx) {
				forward.x = speed.x;
				forward.y = 0.0f;
				if (speed.x < 0.0f) {
					if (int(position.x) % (ENEMY_SPEED_X * 20) < (ENEMY_SPEED_X * 10)) {
						setuv(97, 9, 113, 32);
					}
					else {
						setuv(113, 9, 129, 32);
					}
				}
				else {
					if (int(position.x) % (ENEMY_SPEED_X * 20) < (ENEMY_SPEED_X * 10)) {
						setuv(113, 9, 97, 32);
					}
					else {
						setuv(129, 9, 113, 32);
					}
				}
				turnx = false;
			}
			else {
				speed.y -= ENEMY_SPEED_Y;
				if (speed.y < -TIPY) {
					speed.y = -TIPY;
				}
				forward.y = speed.y;
				forward.x = 0.0f;
				turnx = true;
			}

			position += forward;
			CBase::update();
			break;
		case Active2:	// koura stand
			if (descnt++ > 180)
				enabled = false;
			break;
		case Active3:	// koura move
			if (turnx) {
				forward.x = speed.x;
				forward.y = 0.0f;
				turnx = false;
			}
			else {
				speed.y -= ENEMY_SPEED_Y;
				if (speed.y < -TIPY) {
					speed.y = -TIPY;
				}
				forward.y = speed.y;
				forward.x = 0.0f;
				turnx = true;
			}

			position += forward;
			CBase::update();
			break;
		default:
			if (descnt++ > 560)
				enabled = false;
			break;
		}
	}

	void onCollision(const CBase &b) {
		CVector2 v;
		switch (b.objectType) {
		case Player:
			if (b.jumpFlg == false && b.speed.y < 0.0f) {
				switch (status) {
				case Active:
					speed.x = 0.0f;
					speed.y = 0.0f;
					rect.v1L.y = TIPY - 0.2f;
					rect.v4L.y = TIPY - 0.2f;
					setuv(161, 16, 176, 32);
					CBase::update();
					status = Stop;
					descnt = 0;
					break;
				case Stop:
					descnt = 0;
					forward.x = 0.0f;
					forward.y = 0.0f;
					status = Active2;
					break;
				case Active2:
					descnt = 0;
					if (b.position.x < position.x) {
						speed.x = ENEMY_SPEED_X * 3;
						forward.x = speed.x;
					}
					else {
						speed.x = -ENEMY_SPEED_X * 3;
						forward.x = speed.x;
					}
					status = Active3;
					break;
				case Active3:
					break;
				}
			}
			break;
		case EnemyBlock:
		case GrdBlock:
			if (CCollision::collision(b.rect, rect, forward, &v)) {
				position += v;
				CBase::update();
				if (turnx) {
					speed.y = 0.0f;
				}
				else {
					speed.x = -speed.x;
				}
			}
		}

	}

};

class CTile : public CBase {
public:
	CTile()
		:
		CBase(-TIPX, TIPY, TIPX, -TIPY, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)
	{
		setObjectType(GrdBlock);
	}
};

class CHatena : public CBase {
public:
	int cnt = 0;
	CHatena()
		:
		CBase(-TIPX, TIPY, TIPX, -TIPY, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)
	{
		setObjectType(Hatena);
	}

	void update() {
		int w = 32;
		switch (status) {
		case Active2:	// up and down
			if (cnt < 20) {
				forward.y = 1;
			}
			else if (cnt < 40) {
				forward.y = -1;
			}
			else {
				forward.y = 0;
				status = Active3;
				setuv(434, 1, 448, 16);
				break;
			}
		case Active:	// ?Box
			if (cnt % w < w / 4) {
				setuv(386, 1, 401, 16);
			}
			else if (cnt % w < w * 2 / 4) {
				setuv(402, 1, 417, 16);
			}
			else if (cnt % w < w * 3 / 4) {
				setuv(418, 1, 433, 16);

			}
			else {
				setuv(402, 1, 417, 16);
			}
			cnt %= w * 4;
			break;
		case Active3:	// silent

		default:
			;
		}
		cnt++;
		position += forward;
		CBase::update();
	}

	void onCollision(const CBase &b) {
		CVector2 v;
		switch (b.objectType) {
		case Player:
			if (b.jumpFlg == false && b.speed.y > 0.0f) {	// attack upper
				switch (status) {
				case Active:
					status = Active2;
					cnt = 0;
					break;
				}
			}
			break;
		default:
			break;
		}
	}
};

class CCoin : public CBase {
public:
	int cnt = 0;
	CCoin()
		:
		CBase(-TIPX, TIPY, TIPX, -TIPY, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)
	{
		setObjectType(Coin);
	}

	void update() {
		int w = 32;
		switch (status) {
		case Active2:	// up and down
			if (cnt < 20) {
				forward.y = 1;
			}
			else if (cnt < 40) {
				forward.y = -1;
			}
			else {
				forward.y = 0;
				status = Active3;
				setuv(434, 1, 448, 16);
				break;
			}
		case Active:	// ?Box
			if (cnt % w < w / 4) {
				setuv(386, 16, 401, 32);
			}
			else if (cnt % w < w * 2 / 4) {
				setuv(402, 16, 417, 32);
			}
			else if (cnt % w < w * 3 / 4) {
				setuv(418, 16, 433, 32);

			}
			else {
				setuv(402, 16, 417, 32);
			}
			cnt %= w * 4;
			break;
		case Active3:	// silent

		default:
			;
		}
		cnt++;
		position += forward;
		CBase::update();
	}

	void onCollision(const CBase &b) {
		CVector2 v;
		switch (b.objectType) {
		case Player:
			enabled = false;
			break;
		default:
			break;
		}
	}
};

class CAkakinoko : public CBase {
public:
	CHatena *hatena;
	int cnt = 0;
	CVector2 firstPosition;

	CAkakinoko()
		: CBase(-(TIPX - 0.2f), TIPY - 0.2f, TIPX - 0.2f, -(TIPY - 0.2f), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)
	{
		setObjectType(Akakinoko);
		setSpeed(ENEMY_SPEED_X, 0.0f);
		status = Active;
	}

	void update() {
		int w = 32;
		switch (status) {
		case Active:	// in ?Box
			firstPosition = position;
			break;
		case Active2:	// appear
			forward.y = 1;
			if (firstPosition.y + TIPY * 2 <= position.y) {
				forward.y = 0;
				speed.x = ENEMY_SPEED_X;
				speed.y = 0;
				status = Active3;
			}
			break;
		case Active3:	// move right left
			if (turnx) {
				forward.x = speed.x;
				forward.y = 0.0f;
				turnx = false;
			}
			else {
				speed.y -= ENEMY_SPEED_Y;
				if (speed.y < -TIPY) {
					speed.y = -TIPY;
				}
				forward.y = speed.y;
				forward.x = 0.0f;
				turnx = true;
			}
			break;
		case Stop:	// hit player
			enabled = false;
			break;
		default:
			;
		}
		cnt++;
		cnt %= w;
		position += forward;
		CBase::update();
	}

	void onCollision(const CBase &b) {
		CVector2 v;
		switch (b.objectType) {
		case EnemyBlock:
			break;
		case Player:
			if (status == Active3) {
				status = Stop;
			}
			if (b.jumpFlg == false && b.speed.y > 0.0f) {	// attack upper
				switch (status) {
				case Active:
					status = Active2;
					break;
				}
			}
			break;
		case GrdBlock:
			if (CCollision::collision(b.rect, rect, forward, &v)) {
				position += v;
				CBase::update();
				if (turnx) {
					speed.y = 0.0f;
				}
				else {
					speed.x = -speed.x;
				}
			}
		}
	}

};

class CBackGround : public CBase {
public:
	CBackGround(float left, float top, float right, float bottom,
		float px, float py) 
		:
		CBase(left, top, right, bottom, 1.0f, 1.0f, 1.0f, 1.0f, px, py)
	{
		collisionFlg = false;
	}

	void update() {
		if (position.x < CGame::rangeXS) {
			position.x += XMAX * 2;
		}
		if (position.x > CGame::rangeXE) {
			position.x -= XMAX * 2;
		}
		CBase::update();
	}
};

#include <list>		//���X�g���g�p����
/*
�^�X�N�Ǘ��N���X
�^�X�N�V�X�e���̊Ǘ����s��
*/
class CTaskManager {
private:	//private�͑��̃N���X����A�N�Z�X�ł��Ȃ�
	//�B��̃^�X�N�Ǘ��N���X�̃C���X�^���X�i�V���O���g���j
	static CTaskManager* tm;
	//���X�g�f�[�^�̍쐬
	std::list<CBase*> list;
	//�R���W�������X�g�̍쐬
	std::list<CBase*> collision;
	//�f�t�H���g�R���X�g���N�^
	CTaskManager() {}
public:
	//�f�X�g���N�^
	~CTaskManager() {
	}
	/*
	�^�X�N�Ǘ��N���X�̃C���X�^���X�̎擾
	*/
	static CTaskManager* CTaskManager::GetInstance();
	/*
	�X�V����
	�S�C���X�^���X�̍X�V�A����яՓ˔���
	*/
	void CTaskManager::update() {
		/*�X�V����*/
		std::list<CBase*>::iterator itr;	//���X�g�̃C�e���[�^���擾
		//���X�g�̍ŏ�����Ō�܂ŌJ��Ԃ�
		for (itr = tm->list.begin(); itr != tm->list.end(); itr++) {
			//�C���X�^���X�̗L���t���O�������̏ꍇ
			if ((*itr)->enabled == false) {
				/*
				delete (*itr);	//�C���X�^���X���폜
				itr = list.erase(itr);	//���X�g����폜
				if (itr == tm->list.end())	//���X�g�̍Ō�̏ꍇ�͌J��Ԃ��I��
					break;
				*/
			}
			else {
				(*itr)->update();	//�L���ȏꍇ�A�X�V�������Ă�
			}
		}
		/*�Փ˔���*/
		std::list<CBase*>::iterator itr2;	//���X�g�̃C�e���[�^���擾
		//���X�g�̍ŏ�����Ō�܂ŌJ��Ԃ�
		for (itr = tm->collision.begin(); itr != tm->collision.end(); itr++) {
			//�C���X�^���X���L��
			if ((*itr)->enabled) {
				//�Փ˔���̑��蕪�J��Ԃ�
				for (itr2 = tm->list.begin(); itr2 != tm->list.end(); itr2++) {
					if ((*itr2)->enabled) {
						if ((*itr)->collisionFlg && (*itr2)->collisionFlg) {
							//�����C���X�^���X�͏������Ȃ�
							if ((*itr) != (*itr2)) {
								//�Փ˂��Ă��鎞
								if (CCollision::collision((*itr)->rect, (*itr2)->rect)) {
									(*itr)->onCollision(*(*itr2));	//�Փˏ������Ăяo��
								}
							}
						}
					}
				}
			}
			else {
				/*
				delete (*itr);	//�C���X�^���X���폜
				itr = collision.erase(itr);	//���X�g����폜
				if (itr == tm->collision.end())	//���X�g�̍Ō�̏ꍇ�͌J��Ԃ��I��
					break;
				*/
			}
		}
	}
	/*
	�ǉ�����
	�C���X�^���X�����X�g�ɒǉ�����
	*/
	void CTaskManager::add(CBase* r) {
		list.push_back(r);	//���X�g�ɒǉ�����
	}
	/*
	�ǉ�����
	�C���X�^���X�����X�g�ƃR���W�������X�g�ɒǉ�����
	*/
	void CTaskManager::addCollision(CBase* r) {
		list.push_back(r);	//���X�g�ɒǉ�����
		collision.push_front(r);	//���X�g�ɒǉ�����
	}
	/*
	�`�揈��
	�S�C���X�^���X�̕`�揈�����Ăяo��
	*/
	void render() {
		std::list<CBase*>::iterator itr;	//�C�e���[�^�擾
		//���X�g�̍ŏ�����Ō�܂ŌJ��Ԃ�
		for (itr = tm->list.begin(); itr != tm->list.end(); itr++) {
			//�C���X�^���X���L��
			if ((*itr)->enabled) {
				//��ʓ��̂��̂����`�悷��
				if ((*itr)->position.x >= CGame::rangeXS - TIPX && (*itr)->position.x <= CGame::rangeXE + TIPX) {
					(*itr)->render();	//�`�揈�����Ăяo��
				}
			}
		}
	}
	/*
	�p������
	�S�C���X�^���X�̔j��
	*/
	void destroy() {
		std::list<CBase*>::iterator itr;
		for (itr = tm->list.begin(); itr != tm->list.end(); itr++) {
			delete *itr;	//�C���X�^���X�̍폜
		}
		list.clear();	//���X�g�̃N���A
		collision.clear();
		delete tm;	//�^�X�N�}�l�[�W���̃C���X�^���X�̍폜
		tm = 0;	//�|�C���^�̏�����
	}
};
//�B��̃^�X�N�Ǘ��N���X�̃C���X�^���X�i�V���O���g���j
CTaskManager* CTaskManager::tm = 0;
/*
�^�X�N�Ǘ��N���X�̃C���X�^���X�̎擾
*/
CTaskManager* CTaskManager::GetInstance() {
	//�C���X�^���X���Ȃ���ΐ�������
	if (tm == 0) {
		tm = new CTaskManager();
		tm->list.clear();	//���X�g�̃N���A
	}
	return tm;	//�C���X�^���X�̃|�C���^�l��Ԃ�
}

class CMap {
public:
	int *pmap;
	int size;
	const int HSIZE = 10;
	int s;

	CMap() :
		s(0) {}

	void createMap(CTexture *tile, CTexture *enemy, CTexture *items) {
		int data[] = {
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 9, 9, 9, 9, 9, 9, 9, 9,	//����ȑO�ύX���Ȃ�
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,	//10
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 6, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 4, 0, 2, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 5, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 2, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,	//20
			1, 1, 0, 3, 0, 2, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 3, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 3, 0, 0, 8, 0, 0, 0,
			1, 1, 0, 0, 0, 2, 0, 0, 0, 0,
			1, 1, 0, 3, 0, 2, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 2, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 2, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 2, 3, 0, 0, 0,	//30
			1, 1, 0, 0, 0, 0, 8, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 2, 0, 0, 2, 0,
			1, 1, 0, 0, 0, 2, 0, 0, 2, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,	//40
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 9, 9, 9, 9, 9, 9, 9, 9,	//����ȍ~�ύX���Ȃ�
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		};
		CBase *b;
		CAkakinoko *k;
		for (; s < CGame::rangeXE + TIPX*2; s += TIPX*2) {
			for (int i = (s / (TIPX*2)) * HSIZE; i < (s / (TIPX*2)) * HSIZE + HSIZE; i++) {
				switch (data[i]) {
				case 1:
					b = new CTile();
					b->setPosition(i, HSIZE);
					b->setTexture(tile, 1, 1, 16, 16);
					gettm()->add(b);
					break;
				case 2:
					b = new CTile();
					b->setPosition(i, HSIZE);
					b->setTexture(tile, 33, 1, 48, 16);
					gettm()->add(b);
					break;
				case 3:
					b = new CKuribo();
					b->setPosition(i, HSIZE);
					b->setTexture(enemy, 1, 17, 16, 32);
					gettm()->addCollision(b);
					break;
				case 4:
					b = new CNokonoko();
					b->setPosition(i, HSIZE);
					b->setTexture(enemy, 97, 9, 112, 32);
					gettm()->addCollision(b);
					break;
				case 5:
					b = new CTile();
					b->setPosition(i, HSIZE);
					b->setColor(1.0f, 1.0f, 1.0f, 0.0f);
					gettm()->add(b);

					k = new CAkakinoko();
					k->setPosition(i, HSIZE);
					k->setTexture(items, 1, 16, 16, 32);
					gettm()->addCollision(k);

					b = new CHatena();
					b->setPosition(i, HSIZE);
					b->setTexture(tile, 385, 1, 400, 16);
					gettm()->addCollision(b);
					break;
				case 6:
					b = new CCoin();
					b->setPosition(i, HSIZE);
					b->setTexture(tile, 386, 16, 401, 32);
					gettm()->addCollision(b);
					break;
				case 9:
					b = new CTile();
					b->setPosition(i, HSIZE);
					b->setColor(1.0f, 1.0f, 1.0f, 0.0f);
					gettm()->add(b);
					break;
				}
			}
		}

		return;
	}

	CTaskManager* gettm() {
		return CTaskManager::GetInstance();
	}
};

class CScenegame {
public:
	enum EStatus {
		Init,
		Game
	};
	CBackGround* pbackground;
	CMap map;
	CPlayer* player;

	CTexture texBackGround;
	CTexture texPlayer;
	CTexture texTile;
	CTexture texEnemy;
	CTexture texItems;

	EStatus status;

	CScenegame()
		:
		status(Init)
	{}

	~CScenegame() {
		gettm()->destroy();
	}

	CTaskManager* gettm() {
		return CTaskManager::GetInstance();
	}

	void update() {
		switch (status) {
		case Init:
//			CGame::rangeXS = -XMAX + XMAX + TIPX;
			CGame::rangeXS = 0;
			CGame::rangeXE = XMAX + XMAX + TIPX;

			texBackGround.load("BackGround.tga");
			pbackground = new CBackGround(-XMAX * 2, YMAX, XMAX * 2, -YMAX, XMAX, YMAX);
			pbackground->setTexture(&texBackGround, 1, 1, 1725, 470);
			gettm()->add(pbackground);

			texPlayer.load("Mario.tga");
			player = new CPlayer();
			player->setPosition(XMAX + TIPX, YMAX);
			player->setTexture(&texPlayer, 81, 33, 96, 48);
			player->update();
			gettm()->addCollision(player);

			texTile.load("Tile.tga");
			texEnemy.load("Enemies.tga");
			texItems.load("items.tga");
			map.createMap(&texTile, &texEnemy, &texItems);

			status = Game;
			break;
		case Game:
			gettm()->update();
			gettm()->update();

			map.createMap(&texTile, &texEnemy, &texItems);

			glMatrixMode(GL_PROJECTION);		//�s����v���W�F�N�V�������[�h�֕ύX
			glLoadIdentity();	//�s���������
			gluOrtho2D(CGame::rangeXS, CGame::rangeXE, 0, YMAX*2);	//��ʂ̍��W�n��ݒ�
			glMatrixMode(GL_MODELVIEW);		//�s������f���r���[���[�h�֕ύX
			glLoadIdentity();	//�s���������

			gettm()->render();
			break;
		}
	}

};

CScenegame scenegame;

/* display�֐�
1�b�Ԃ�60����s�����
*/
void display() {
	//��ʂ̃N���A
	glClear(GL_COLOR_BUFFER_BIT);

	scenegame.update();

	//��ʂɕ\��
	glutSwapBuffers();
}

//�֐��̃v���g�^�C�v�錾
void reshape(int, int);
void idle();

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	//�E�B���h�E�̐ݒ�
	glutInitWindowSize(WINDOW_PIXEL_H, WINDOW_PIXEL_V);	//�E�B���h�E�T�C�Y�ݒ�
	glutInitDisplayMode(GLUT_RGBA | GL_DOUBLE);	//�f�B�X�v���C���[�h�ݒ�
	glutCreateWindow("Hello GameProgramming");	//�E�B���h�E�쐬
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);	//�N���A�F���O���[�ɐݒ�

	//�e��֐��ݒ�
	glutDisplayFunc(display);	//�`�掞�̊֐����w��
	glutReshapeFunc(reshape);	//�E�B���h�E�ό`���̊֐����w��
	glutIdleFunc(idle);			//�ҋ@���̊֐����w��

	//���C�����[�v
	glutMainLoop();

	return;
}

/*�E�B���h�E�T�C�Y�ύX���̏���
void reshape(int width, int height)
width:��ʕ�
height:��ʍ���
*/
void reshape(int width, int height) {
	glViewport(0, 0, width, height);	//��ʂ̕`��G���A�̎w��
	glMatrixMode(GL_PROJECTION);		//�s����v���W�F�N�V�������[�h�֕ύX
	gluOrtho2D(-XMAX, XMAX, -YMAX, YMAX);	//��ʂ̍��W�n��ݒ�
	glMatrixMode(GL_MODELVIEW);		//�s������f���r���[���[�h�֕ύX
	glLoadIdentity();	//�s���������
}

LARGE_INTEGER last_time;	//�O��̃J�E���^�l
//�P�b�ԂɂU�O��`�悷��悤�ɒ��߂���
void idle() {
	LARGE_INTEGER freq;		//��b������̃J�E���^��
	LARGE_INTEGER time;		//����̃J�E���^�l

	//��b�Ԃ̃J�E���^�����擾
	QueryPerformanceFrequency(&freq);

	if (last_time.QuadPart == 0) {
		QueryPerformanceCounter(&last_time);
	}
	do{
		//���݂̃V�X�e���̃J�E���g�����擾
		QueryPerformanceCounter(&time);

		//���̃J�E���g-�O��̃J�E���g < 1�b������̃J�E���g����60�Ŋ���(1/60�b������̃J�E���g��)
	} while (time.QuadPart - last_time.QuadPart < freq.QuadPart / 60);
	last_time = time;

	//�`�悷��֐����Ă�
	display();
}
