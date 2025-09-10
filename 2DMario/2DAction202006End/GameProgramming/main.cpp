#include "define.h"
#include "glut.h"
#define _USE_MATH_DEFINES	//数学ライブラリのM_PIを有効にする
#include <math.h>	//数学ライブラリのインクルード
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

/* 2次元のベクトルクラスを作成する */
class CVector2 {
public:
	float x;	//X軸への大きさ
	float y;	//Y軸への大きさ
	float d;	//ダミー

	CVector2() 
		:
		x(0.0), y(0.0f), d(0.0f) {};	//デフォルトコンストラクタ
	CVector2(float x, float y) 
		:
		x(x), y(y), d(0.0f) {};	//コンストラクタ
	CVector2(float x, float y, float d) 
		:
		x(x), y(y), d(d) {};	//コンストラクタ

	/*　+=演算子のオーバーライド
	CVector2 += CVector2　の宣言
	*/
	void operator+=(const CVector2 &v) {
		x += v.x; y += v.y;
	}
	/*　*=演算子のオーバーライド
	CVector2 *= CVector2　の宣言
	*/
	void operator*=(const CVector2 &v) {
		x *= v.x; y *= v.y;
	}
	/*　*=演算子のオーバーライド
	CVector2 *= float　の宣言
	*/
	void operator*=(float f) {
		x *= f; y *= f;
	}
	/*　*演算子のオーバーライド
	CVector2 * float　の宣言
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
TGAファイルのヘッダフォーマット
*/
struct STgaheader {
	unsigned char	dummy1[12];
	unsigned short	width;		//幅
	unsigned short	height;	//高さ
	unsigned char	depth;	//ビットの深さ
	unsigned char	dummy2;
};
/*
テクスチャークラス
*/
class CTexture {
public:
	//テクスチャID
	GLuint id;
	//TGAファイルのヘッダ情報
	STgaheader header;
	/*
	デフォルトコンストラクタ
	*/
	CTexture() : id(0) {}
	/*
	デストラクタ（このインスタンスが破棄されるときに実行される）
	*/
	~CTexture() {
		//テクスチャデータがあれば削除する
		if (id != 0) {
			//テクスチャデータの削除
			glDeleteTextures(1, &id);
			id = 0;
		}
	}

	/*
	画像ファイルの読み込み（テクスチャファイルの読み込み）
	*/
	void load(const char* filename) {
		//画像データ
		unsigned char* data;
		//ファイルポインタの作成
		FILE *fp;
		//ファイルオープン
		fp = fopen(filename, "rb");
		//エラーのときはリターン
		if (!fp) {
			printf("file not found:%s\n", filename);
			return;
		}
		//ヘッダ情報を読み込む
		fread(&header, sizeof(header), 1, fp);
		//画像データのバイト数を計算する
		int length = header.width * header.height * header.depth / 8;
		//画像データの格納領域を確保
		data = new unsigned char[length];
		//画像データをヘッダの続きから読み込む
		fread(data, length, 1, fp);
		//画像ファイルのクローズ
		fclose(fp);

		//テクスチャデータの作成
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (header.depth == 32)
			//アルファ有りのテクスチャ作成
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, header.width,
			header.height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
		else
			//アルファ無しのテクスチャ作成
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, header.width,
			header.height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
		//格納領域を解放
		delete[] data;
	}

};
/* 長方形クラス */
class  CRectangle {
public:
	//座標
	CVector2 v1L, v2L, v3L, v4L;
	CVector2 v1W, v2W, v3W, v4W;

	//色
	float r, g, b, a;	//r:赤 g:緑 b:青 a:アルファ（透明度）

	//テクスチャ
	CTexture	*ptexture;	//テクスチャのポインタ
	float		uL, uR, vT, vB;	//テクスチャマッピングの値

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

	//描画する
	void render() {
		//アルファブレンドを有効にする
		glEnable(GL_BLEND);
		//ブレンド方法を指定
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//テクスチャの有無
		if (ptexture == 0) {
			//描画色を指定
			glColor4f(r, g, b, a);
			//テクスチャの無いとき
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
			//テクスチャが有るとき
			//テクスチャを有効にする
			glEnable(GL_TEXTURE_2D);
			//テクスチャを指定
			glBindTexture(GL_TEXTURE_2D, ptexture->id);

			//描画色を指定
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

			//テクスチャを解放
			glBindTexture(GL_TEXTURE_2D, 0);
			//テクスチャを無効
			glDisable(GL_TEXTURE_2D);
		}
		//アルファブレンドを無効
		glDisable(GL_BLEND);
	}

	/*
	テクスチャ情報の設定
	*t		CTextureのインスタンスのポインタ
	left	画像の左上端を原点(0,0)とし、貼り付けたい領域の左の座標値
	top		画像の左上端を原点(0,0)とし、貼り付けたい領域の上の座標値
	right	画像の左上端を原点(0,0)とし、貼り付けたい領域の右の座標値
	bottom	画像の左上端を原点(0,0)とし、貼り付けたい領域の下の座標値
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
	四角形r1と四角形r2がfwdだけ移動し重なった場合、rtnだけ戻せば重ならない
	*/
	static bool collision(const CRectangle &r1, const CRectangle &r2, const CVector2 &fwd, CVector2 *rtn);
	//四角形と線分
	static bool collision(const CVector2 &v1, const CVector2 &v2, const CVector2 &v3, const CVector2 &v4, const CVector2 &s, const CVector2 &e, CVector2 *t);
	//線分V11V12と線分V21V22の交差判定
	static bool collision(const CVector2 &v11, const CVector2 &v12, const CVector2 &v21, const CVector2 &v22, CVector2 *rt);

};

bool CCollision::collision(CRectangle &r1, CRectangle &r2){
	if (r1.v1W.x < r2.v3W.x && r2.v1W.x < r1.v3W.x && r1.v1W.y > r2.v3W.y && r1.v3W.y < r2.v1W.y){
		return true;
	}
	return false;
}
/*
四角形r1と四角形r2がfwdだけ移動し重なった場合、rtnだけ戻せば重ならない
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

//四角形と線分
bool CCollision::collision(const CVector2 &v1, const CVector2 &v2, const CVector2 &v3, const CVector2 &v4, const CVector2 &s, const CVector2 &e, CVector2 *t) {
	//V12と交差するか判定
	if (collision(v1, v2, s, e, t)) {
		return true;
	}
	//V23と交差するか判定
	if (collision(v2, v3, s, e, t)) {
		return true;
	}
	//V34と交差するか判定
	if (collision(v3, v4, s, e, t)) {
		return true;
	}
	//V41と交差するか判定
	if (collision(v4, v1, s, e, t)) {
		return true;
	}
	//交差していない
	return false;
}

//線分V11V12と線分V21V22の交差判定
bool CCollision::collision(const CVector2 &v11, const CVector2 &v12, const CVector2 &v21, const CVector2 &v22, CVector2 *rt) {
	CVector2 v1112 = v12 + v11 * -1;
	CVector2 n1112;
	n1112.x = -v1112.y;
	n1112.y = v1112.x;
	n1112.normalize();
	float dots = n1112.dot(v21 + v11 * -1);
	float dote = n1112.dot(v22 + v11 * -1);
	//プラスだったら交差していない
	if (dots * dote > 0) {
		return false;
	}
	else {
		//始点が外は交差なしとする
		if (dots < 0) {
			return false;
		}
		//両方0だと線上。線上は衝突していない。
		if (dots == 0.0f && dote == 0.0f) {
			return false;
		}
		dots = -dots / (-dots + dote);
	}
	CVector2 v5 = v22 + v21 * -1;
	v5 = v5 * dots;
	v5 = v21 + v5;
	//交点v5がv1112上か
	float length = v1112.length();
	v1112.normalize();
	dote = v1112.dot(v5 + v11*-1);
	//線上は交差していない
	if (dote <= 0 || length <= dote){
		return false;
	}
	//交差確定（戻し量計算）
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
					jumpFlg = true;	//ジャンプ可能
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

#include <list>		//リストを使用する
/*
タスク管理クラス
タスクシステムの管理を行う
*/
class CTaskManager {
private:	//privateは他のクラスからアクセスできない
	//唯一のタスク管理クラスのインスタンス（シングルトン）
	static CTaskManager* tm;
	//リストデータの作成
	std::list<CBase*> list;
	//コリジョンリストの作成
	std::list<CBase*> collision;
	//デフォルトコンストラクタ
	CTaskManager() {}
public:
	//デストラクタ
	~CTaskManager() {
	}
	/*
	タスク管理クラスのインスタンスの取得
	*/
	static CTaskManager* CTaskManager::GetInstance();
	/*
	更新処理
	全インスタンスの更新、および衝突判定
	*/
	void CTaskManager::update() {
		/*更新処理*/
		std::list<CBase*>::iterator itr;	//リストのイテレータを取得
		//リストの最初から最後まで繰り返し
		for (itr = tm->list.begin(); itr != tm->list.end(); itr++) {
			//インスタンスの有効フラグが無効の場合
			if ((*itr)->enabled == false) {
				/*
				delete (*itr);	//インスタンスを削除
				itr = list.erase(itr);	//リストから削除
				if (itr == tm->list.end())	//リストの最後の場合は繰り返し終了
					break;
				*/
			}
			else {
				(*itr)->update();	//有効な場合、更新処理を呼ぶ
			}
		}
		/*衝突判定*/
		std::list<CBase*>::iterator itr2;	//リストのイテレータを取得
		//リストの最初から最後まで繰り返し
		for (itr = tm->collision.begin(); itr != tm->collision.end(); itr++) {
			//インスタンスが有効
			if ((*itr)->enabled) {
				//衝突判定の相手分繰り返す
				for (itr2 = tm->list.begin(); itr2 != tm->list.end(); itr2++) {
					if ((*itr2)->enabled) {
						if ((*itr)->collisionFlg && (*itr2)->collisionFlg) {
							//同じインスタンスは処理しない
							if ((*itr) != (*itr2)) {
								//衝突している時
								if (CCollision::collision((*itr)->rect, (*itr2)->rect)) {
									(*itr)->onCollision(*(*itr2));	//衝突処理を呼び出す
								}
							}
						}
					}
				}
			}
			else {
				/*
				delete (*itr);	//インスタンスを削除
				itr = collision.erase(itr);	//リストから削除
				if (itr == tm->collision.end())	//リストの最後の場合は繰り返し終了
					break;
				*/
			}
		}
	}
	/*
	追加処理
	インスタンスをリストに追加する
	*/
	void CTaskManager::add(CBase* r) {
		list.push_back(r);	//リストに追加する
	}
	/*
	追加処理
	インスタンスをリストとコリジョンリストに追加する
	*/
	void CTaskManager::addCollision(CBase* r) {
		list.push_back(r);	//リストに追加する
		collision.push_front(r);	//リストに追加する
	}
	/*
	描画処理
	全インスタンスの描画処理を呼び出す
	*/
	void render() {
		std::list<CBase*>::iterator itr;	//イテレータ取得
		//リストの最初から最後まで繰り返し
		for (itr = tm->list.begin(); itr != tm->list.end(); itr++) {
			//インスタンスが有効
			if ((*itr)->enabled) {
				//画面内のものだけ描画する
				if ((*itr)->position.x >= CGame::rangeXS - TIPX && (*itr)->position.x <= CGame::rangeXE + TIPX) {
					(*itr)->render();	//描画処理を呼び出す
				}
			}
		}
	}
	/*
	廃棄処理
	全インスタンスの破棄
	*/
	void destroy() {
		std::list<CBase*>::iterator itr;
		for (itr = tm->list.begin(); itr != tm->list.end(); itr++) {
			delete *itr;	//インスタンスの削除
		}
		list.clear();	//リストのクリア
		collision.clear();
		delete tm;	//タスクマネージャのインスタンスの削除
		tm = 0;	//ポインタの初期化
	}
};
//唯一のタスク管理クラスのインスタンス（シングルトン）
CTaskManager* CTaskManager::tm = 0;
/*
タスク管理クラスのインスタンスの取得
*/
CTaskManager* CTaskManager::GetInstance() {
	//インスタンスがなければ生成する
	if (tm == 0) {
		tm = new CTaskManager();
		tm->list.clear();	//リストのクリア
	}
	return tm;	//インスタンスのポインタ値を返す
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
			1, 1, 9, 9, 9, 9, 9, 9, 9, 9,	//これ以前変更しない
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
			1, 1, 9, 9, 9, 9, 9, 9, 9, 9,	//これ以降変更しない
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

			glMatrixMode(GL_PROJECTION);		//行列をプロジェクションモードへ変更
			glLoadIdentity();	//行列を初期化
			gluOrtho2D(CGame::rangeXS, CGame::rangeXE, 0, YMAX*2);	//画面の座標系を設定
			glMatrixMode(GL_MODELVIEW);		//行列をモデルビューモードへ変更
			glLoadIdentity();	//行列を初期化

			gettm()->render();
			break;
		}
	}

};

CScenegame scenegame;

/* display関数
1秒間に60回実行される
*/
void display() {
	//画面のクリア
	glClear(GL_COLOR_BUFFER_BIT);

	scenegame.update();

	//画面に表示
	glutSwapBuffers();
}

//関数のプロトタイプ宣言
void reshape(int, int);
void idle();

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	//ウィンドウの設定
	glutInitWindowSize(WINDOW_PIXEL_H, WINDOW_PIXEL_V);	//ウィンドウサイズ設定
	glutInitDisplayMode(GLUT_RGBA | GL_DOUBLE);	//ディスプレイモード設定
	glutCreateWindow("Hello GameProgramming");	//ウィンドウ作成
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);	//クリア色をグレーに設定

	//各種関数設定
	glutDisplayFunc(display);	//描画時の関数を指定
	glutReshapeFunc(reshape);	//ウィンドウ変形時の関数を指定
	glutIdleFunc(idle);			//待機時の関数を指定

	//メインループ
	glutMainLoop();

	return;
}

/*ウィンドウサイズ変更時の処理
void reshape(int width, int height)
width:画面幅
height:画面高さ
*/
void reshape(int width, int height) {
	glViewport(0, 0, width, height);	//画面の描画エリアの指定
	glMatrixMode(GL_PROJECTION);		//行列をプロジェクションモードへ変更
	gluOrtho2D(-XMAX, XMAX, -YMAX, YMAX);	//画面の座標系を設定
	glMatrixMode(GL_MODELVIEW);		//行列をモデルビューモードへ変更
	glLoadIdentity();	//行列を初期化
}

LARGE_INTEGER last_time;	//前回のカウンタ値
//１秒間に６０回描画するように調節する
void idle() {
	LARGE_INTEGER freq;		//一秒当たりのカウンタ数
	LARGE_INTEGER time;		//今回のカウンタ値

	//一秒間のカウンタ数を取得
	QueryPerformanceFrequency(&freq);

	if (last_time.QuadPart == 0) {
		QueryPerformanceCounter(&last_time);
	}
	do{
		//現在のシステムのカウント数を取得
		QueryPerformanceCounter(&time);

		//今のカウント-前回のカウント < 1秒当たりのカウント数を60で割る(1/60秒当たりのカウント数)
	} while (time.QuadPart - last_time.QuadPart < freq.QuadPart / 60);
	last_time = time;

	//描画する関数を呼ぶ
	display();
}
