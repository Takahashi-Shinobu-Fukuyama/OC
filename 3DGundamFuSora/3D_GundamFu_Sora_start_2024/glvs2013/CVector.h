#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
//�x�N�g���̒�`�Ɗe��v�Z
struct SVector2D{
	union {
		float v[2];
		struct {
			float x;
			float y;
		};
	};
};
class CVector2D : public SVector2D{
public:
	CVector2D(){}
	CVector2D( float x, float y) {this->x = x; this->y = y;}
	CVector2D( const CVector2D& v ) { x = v.x; y = v.y; }
	//�x�N�g�������Z( this - v )
	CVector2D operator - ( const CVector2D& v ) const { return CVector2D( x - v.x, y - v.y ); }
	//�x�N�g�������Z( this - v )
	void operator -= ( const CVector2D& v )  { x -= v.x; y -= v.y;  }
	
	//�x�N�g�������Z( this + v )
	CVector2D operator + ( const CVector2D& v ) const { return CVector2D( x + v.x, y + v.y); }
	
	//�x�N�g�������Z( this + v )
	void operator += ( const CVector2D& v )  {  x += v.x; y += v.y; }

	//�x�N�g���|���Z( this * f )
	CVector2D operator * ( const float& f ) const { return CVector2D( x * f, y * f); }
	//�x�N�g���|���Z( this * f )
	void operator *= ( const float& f ) { x *= f; y *= f; }
	//�x�N�g������Z( this * f )
	CVector2D operator / ( const float& f ) const { return CVector2D( x / f, y / f); }
	
	float Dot(const CVector2D& v)const{
		return x*v.x + y*v.y;
	}
	static float Dot(const CVector2D& v1,const CVector2D& v2) {
		return v1.x*v2.x + v1.y*v2.y;
	}

	
	float Length() {
		return pow( ( x * x ) + ( y * y ), 0.5f );//�x�N�g���̒���;
	}
	float LengthSq() {
		return ( x * x ) + ( y * y );
	}
	//���g��P�ʃx�N�g���ɂ���
	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
	}
	//�P�ʃx�N�g����ԋp
	CVector2D GetNormalize() const {
		CVector2D out = *this;
		out.Normalize();
		return out;
	}
	

};


typedef struct SVector3D{
	union {
		float v[3];
		struct {
			float x;
			float y;
			float z;
		};
		struct {
			float r;
			float g;
			float b;
		};
	};
}SColorRGB;

class CVector3D : public SVector3D{
public:
	
	CVector3D(){}
	CVector3D( float x, float y, float z) {this->x = x; this->y = y; this->z = z; }
	CVector3D( const CVector3D& v ) { x = v.x; y = v.y; z = v.z; }
	
	//�x�N�g�������Z( this - v )
	CVector3D operator - ( const CVector3D& v ) const { return CVector3D( x - v.x, y - v.y, z - v.z ); }
	//�x�N�g�������Z( this - v )
	void operator -= ( const CVector3D& v )  { x -= v.x; y -= v.y; z -= v.z;  }
	
	//�x�N�g�������Z( this + v )
	CVector3D operator + ( const CVector3D& v ) const { return CVector3D( x + v.x, y + v.y, z + v.z ); }
	
	//�x�N�g�������Z( this + v )
	void operator += ( const CVector3D& v )  {  x += v.x; y += v.y; z += v.z; }

	//�x�N�g���|���Z( this * f )
	CVector3D operator * ( const float& f ) const { return CVector3D( x * f, y * f, z * f ); }
	//�x�N�g���|���Z( this * f )
	void operator *= ( const float& f ) { x *= f; y *= f; z *= f; }
	//�x�N�g������Z( this * f )
	CVector3D operator / ( const float& f ) const { return CVector3D( x / f, y / f, z / f ); }
	
	//�x�N�g���O��( this �~ vr )
	CVector3D operator * ( const CVector3D& vr ) const { return CVector3D( (y * vr.z) - (z * vr.y), (z * vr.x) - (x * vr.z), (x * vr.y) - (y * vr.x) ); }
	
	float Dot(const CVector3D& v)const{
		return x*v.x + y*v.y + z*v.z;
	}
	static float Dot(const CVector3D& v1,const CVector3D& v2) {
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
	CVector3D Cross(CVector3D& v)const {
		return CVector3D(y*v.z-z*v.y, z*v.x - x*v.z , x*v.y - y*v.x);
	}
	static CVector3D Cross(const CVector3D& v1,const CVector3D& v2) {
		return CVector3D(v1.y*v2.z-v1.z*v2.y, v1.z*v2.x - v1.x*v2.z , v1.x*v2.y - v1.y*v2.x);
	}
	
	float Length() {
		return pow( ( x * x ) + ( y * y ) + ( z * z ), 0.5f );//�x�N�g���̒���;
	}
	float LengthSq() {
		return ( x * x ) + ( y * y ) + ( z * z );
	}
	//���g��P�ʃx�N�g���ɂ���
	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
	}
	//�P�ʃx�N�g����ԋp
	CVector3D GetNormalize() const {
		CVector3D out = *this;
		out.Normalize();
		return out;
	}

	bool operator == (const CVector3D &v) const { return (x == v.x && y == v.y && z == v.z); };

};

typedef struct SVector4D{
	union {
		float v[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	};
}SColorRGBA;

class CVector4D : public SVector4D{
public:
	CVector4D(){}
	CVector4D(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }
	CVector4D(CVector3D &v) { x = v.x; y = v.y, z = v.z; w = 1.0f; }
};
typedef CVector4D CColorRGBA;

