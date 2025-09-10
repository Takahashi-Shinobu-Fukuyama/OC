#include "CMatrix.h"


void CMatrix::SetIndentity() {
	memset(f,0,sizeof(float)*16);
	m00=1.0f;
	m11=1.0f;
	m22=1.0f;
	m33=1.0f;
}
void CMatrix::Transelate(float x,float y, float z) {
	m03 = x;
	m13 = y;
	m23 = z;
}
void CMatrix::Scale(float x,float y, float z) {
	m00=x;
	m11=y;
	m22=z;
}
void CMatrix::RotationX(float r){
	m11 = cos(r);
	m22 = cos(r);
	m12 =-sin(r);
	m21 = sin(r);
}
void CMatrix::RotationY(float r){
	m00 = cos(r);
	m22 = cos(r);
	m20 =-sin(r);
	m02 = sin(r);
}
void CMatrix::RotationZ(float r) {
	m00 = cos(r);
	m11 = cos(r);
	m01 =-sin(r);
	m10 = sin(r);
}

void CMatrix::RotationAxis(float vx,float vy,float vz,float r){
	float sine	=sin(r);
	float cosine=cos(r);

	m00=vx*vx*(1.0f-cosine)+cosine;
	m01=vx*vy*(1.0f-cosine)-vz*sine;
	m02=vx*vz*(1.0f-cosine)+vy*sine;

	m10=vy*vx*(1.0f-cosine)+vz*sine;
	m11=vy*vy*(1.0f-cosine)+cosine;
	m12=vy*vz*(1.0f-cosine)-vx*sine;

	m20=vz*vx*(1.0f-cosine)-vy*sine;
	m21=vz*vy*(1.0f-cosine)+vx*sine;
	m22=vz*vz*(1.0f-cosine)+cosine;

}

void CMatrix::LookAt(const CVector3D& eye,const CVector3D& at,const CVector3D& up){
	
	CVector3D f = (eye - at).GetNormalize();
	CVector3D r = CVector3D::Cross(up,f).GetNormalize();
	CVector3D u = CVector3D::Cross(f,r).GetNormalize();
	CVector3D t = at-eye;
	m00 = r.x; m01 = r.y; m02 = r.z;
	m10 = u.x; m11 = u.y; m12 = u.z;
	m20 = f.x; m21 = f.y; m22 = f.z;
	m03 = -(eye.x * r.x + eye.y * r.y + eye.z * r.z);
	m13 = -(eye.x * u.x + eye.y * u.y + eye.z * u.z);
	m23 = -(eye.x * f.x + eye.y * f.y + eye.z * f.z);

}
CMatrix CMatrix::operator +(const CMatrix& num){
	CMatrix ans;
	
	for(int i=0;i<16;i++)
		ans.f[i]=f[i]+num.f[i];

	return ans;
}
CMatrix CMatrix::operator *(CMatrix num){
	CMatrix ans;


	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			ans.m[i][j] =0.0f;

			//s—ñÏ
			for(int k=0;k<4;k++){
				ans.m[i][j] += num.m[i][k]*m[k][j];
			}
		}
	}

	return ans;
}
CMatrix CMatrix::operator *(const float num){
	CMatrix ans;
	
	for(int i=0;i<16;i++)
		ans.f[i]=f[i]*num;

	return ans;
}
CMatrix operator *(const float num,const CMatrix mtr){
	CMatrix ans;
	
	for(int i=0;i<16;i++)
		ans.f[i]=mtr.f[i]*num;

	return ans;
}

CVector4D CMatrix::operator * (const CVector4D num){
	CVector4D ans;
	for(int i=0;i<4;i++) {
		ans.v[i] = 0;
		for(int j=0;j<4;j++) {
			ans.v[i]+=m[j][i]*num.v[j];
		}
	}
	return ans;
}
CVector3D CMatrix::operator * (const CVector3D num){
	CVector3D ans;
	for(int i=0;i<3;i++) {
		ans.v[i] = 0;
		for(int j=0;j<3;j++) {
			ans.v[i]+=m[j][i]*num.v[j];
		}
	}
	return ans;
}

CVector4D operator * (const CVector4D num,const CMatrix mtr){
	CVector4D ans;
	for(int i=0;i<4;i++) {
		ans.v[i] = 0;
		for(int j=0;j<4;j++) {
			ans.v[i]+=num.v[j] * mtr.m[j][i];
		}
	}
	return ans;
}
CVector3D operator * (const CVector3D num,const CMatrix mtr){
	CVector3D ans;
	for(int i=0;i<3;i++) {
		ans.v[i] = 0;
		for(int j=0;j<3;j++) {
			ans.v[i]+=num.v[j] * mtr.m[j][i];
		}
	}
	return ans;
}

void CMatrix::setQuaternion(float x,float y, float z,float w){
/*
	float sx = x * x;
    float sy = y * y;
    float sz = z * z;
    float cx = y * z;
    float cy = x * z;
    float cz = x * y;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;


	m00 = 1.0 - 2.0 * (sy + sz);
    m10 = 2.0 * (cz + wz);
    m20 = 2.0 * (cy - wy);
    m01 = 2.0 * (cz - wz);
    m11 = 1.0 - 2.0 * (sx + sz);
    m21 = 2.0 * (cx + wx);
    m02 = 2.0 * (cy + wy);
    m12 - 2.0 * (cx - wx);
    m22 = 1.0 - 2.0 * (sx + sy);
    m03 = 0.0;
    m13 = 0.0;
    m23 = 0.0;
*/
	m00 = 1-2*y*y-2*z*z;	m01 = 2*x*y+2*w*z;		m02 = 2*x*z-2*w*y;		m03 = 0;
	m10 = 2*x*y-2*w*z;		m11 = 1-2*x*x-2*z*z;	m12 = 2*y*z+2*w*x;		m13 = 0;
	m20 = 2*x*z+2*w*y;		m21 = 2*y*z-2*w*x;		m22 = 1-2*x*x-2*y*y;	m23 = 0;
	m30 = 0;				m31 = 0;				m32 = 0;				m33 = 1;
}

/** ‹ts—ñŽæ“¾ **/
CMatrix CMatrix::getInverse( void )
{
	float det = 0.0f;
	det += m00*m11*m22*m33 + m00*m12*m23*m31 + m00*m13*m21*m32;
	det += m01*m10*m23*m32 + m01*m12*m20*m33 + m01*m13*m22*m30;
	det += m02*m10*m21*m33 + m02*m11*m23*m30 + m02*m13*m20*m31;
	det += m03*m10*m22*m31 + m03*m11*m20*m32 + m03*m12*m21*m30;

	det -= m00*m11*m23*m32 + m00*m12*m21*m33 + m00*m13*m22*m31;
	det -= m01*m10*m22*m33 + m01*m12*m23*m30 + m01*m13*m20*m32;
	det -= m02*m10*m23*m31 + m02*m11*m20*m33 + m02*m13*m21*m30;
	det -= m03*m10*m21*m32 + m03*m11*m22*m30 + m03*m12*m20*m31;

	CMatrix b;

	b.m00 = m11*m22*m33 + m12*m23*m31 + m13*m21*m32 - m11*m23*m32 - m12*m21*m33 - m13*m22*m31;
	b.m01 = m01*m23*m32 + m02*m21*m33 + m03*m22*m31 - m01*m22*m33 - m02*m23*m31 - m03*m21*m32;
	b.m02 = m01*m12*m33 + m02*m13*m31 + m03*m11*m32 - m01*m13*m32 - m02*m11*m33 - m03*m12*m31;
	b.m03 = m01*m13*m22 + m02*m11*m23 + m03*m12*m21 - m01*m12*m23 - m02*m13*m21 - m03*m11*m22;

	b.m10 = m10*m23*m32 + m12*m20*m33 + m13*m22*m30 - m10*m22*m33 - m12*m23*m30 - m13*m20*m32;
	b.m11 = m00*m22*m33 + m02*m23*m30 + m03*m20*m32 - m00*m23*m32 - m02*m20*m33 - m03*m22*m30;
	b.m12 = m00*m13*m32 + m02*m10*m33 + m03*m12*m30 - m00*m12*m33 - m02*m13*m30 - m03*m10*m32;
	b.m13 = m00*m12*m23 + m02*m13*m20 + m03*m10*m22 - m00*m13*m22 - m02*m10*m23 - m03*m12*m20;

	b.m20 = m10*m21*m33 + m11*m23*m30 + m13*m20*m31 - m10*m23*m31 - m11*m20*m33 - m13*m21*m30;
	b.m21 = m00*m23*m31 + m01*m20*m33 + m03*m21*m30 - m00*m21*m33 - m01*m23*m30 - m03*m20*m31;
	b.m22 = m00*m11*m33 + m01*m13*m30 + m03*m10*m31 - m00*m13*m31 - m01*m10*m33 - m03*m11*m30;
	b.m23 = m00*m13*m21 + m01*m10*m23 + m03*m11*m20 - m00*m11*m23 - m01*m13*m20 - m03*m10*m21;

	b.m30 = m10*m22*m31 + m11*m20*m32 + m12*m21*m30 - m10*m21*m32 - m11*m22*m30 - m12*m20*m31;
	b.m31 = m00*m21*m32 + m01*m22*m30 + m02*m20*m31 - m00*m22*m31 - m01*m20*m32 - m02*m21*m30;
	b.m32 = m00*m12*m31 + m01*m10*m32 + m02*m11*m30 - m00*m11*m32 - m01*m12*m30 - m02*m10*m31;
	b.m33 = m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m00*m12*m21 - m01*m10*m22 - m02*m11*m20;

	CMatrix tmp = b * (1/det);
	CMatrix nml;
	nml = (*this) * (*this);
	CMatrix tnml;
	tnml = tmp * (*this);

	return tmp;
};