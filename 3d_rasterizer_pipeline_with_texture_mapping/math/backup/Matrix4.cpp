/* Start Header -------------------------------------------------------
File Name: Matrix4.cpp
Purpose: Contains definition for Matrix4
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_1
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/10/2013
- End Header --------------------------------------------------------*/
#include "Matrix4.h"

#include "Point4.h"
#include "Vector4.h"

#include <cmath>

Matrix4::Matrix4(void) {
	Zero() ;
	// I prefer to Identity, will change for my use later
	// Identity() ;
}

Matrix4::Matrix4(const Matrix4& rhs) {
	for (u32 i = 0; i < sizeof(v) / sizeof(*v); ++i) {
		v[i] = rhs.v[i] ;
	}
}
  
Matrix4::Matrix4(f32 mm00, f32 mm01, f32 mm02, f32 mm03,
				 f32 mm10, f32 mm11, f32 mm12, f32 mm13,
				 f32 mm20, f32 mm21, f32 mm22, f32 mm23,
				 f32 mm30, f32 mm31, f32 mm32, f32 mm33) {
	m00 = mm00 ; m01 = mm01 ; m02 = mm02 ; m03 = mm03 ;
	m10 = mm10 ; m11 = mm11 ; m12 = mm12 ; m13 = mm13 ;
	m20 = mm20 ; m21 = mm21 ; m22 = mm22 ; m23 = mm23 ;
	m30 = mm30 ; m31 = mm31 ; m32 = mm32 ; m33 = mm33 ;
}
 
Matrix4& Matrix4::operator=(const Matrix4& rhs) {
	for (u32 i = 0; i < sizeof(v) / sizeof(*v); ++i) {
		v[i] = rhs.v[i] ;
	}
	return *this ;
}
  
Vector4 Matrix4::operator*(const Vector4& rhs) const {
	return Vector4(m00 * rhs.x + m01 * rhs.y  + m02 * rhs.z + m03 * rhs.w,
				   m10 * rhs.x + m11 * rhs.y  + m12 * rhs.z + m13 * rhs.w,
				   m20 * rhs.x + m21 * rhs.y  + m22 * rhs.z + m23 * rhs.w,
				   m30 * rhs.x + m31 * rhs.y  + m32 * rhs.z + m33 * rhs.w) ;
}

Point4  Matrix4::operator*(const Point4& rhs) const{
	return Point4(m00 * rhs.x + m01 * rhs.y  + m02 * rhs.z + m03 * rhs.w,
				  m10 * rhs.x + m11 * rhs.y  + m12 * rhs.z + m13 * rhs.w,
				  m20 * rhs.x + m21 * rhs.y  + m22 * rhs.z + m23 * rhs.w,
				  m30 * rhs.x + m31 * rhs.y  + m32 * rhs.z + m33 * rhs.w) ;
}
  
Matrix4 Matrix4::operator+(const Matrix4& rhs) const {
	Matrix4 mat ;
	mat.Zero() ;

	for (u32 i = 0; i < sizeof(v) / sizeof(*v); ++i) {
		mat.v[i] = v[i] + rhs.v[i] ;
	}
	return mat ;
}

Matrix4 Matrix4::operator-(const Matrix4& rhs) const {
	Matrix4 mat ;
	mat.Zero() ;

	for (u32 i = 0; i < sizeof(v) / sizeof(*v); ++i) {
		mat.v[i] = v[i] - rhs.v[i] ;
	}
	return mat ;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
	Matrix4 mat ;
	mat.Zero() ;

	for (u32 i = 0; i < 4; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			for (u32 k = 0; k < 4; ++k) {
				mat.m[i][j] += m[i][k] * rhs.m[k][j] ;
			}
		}
	}

	return mat ;
}
  
Matrix4& Matrix4::operator+=(const Matrix4& rhs) {
	for (u32 i = 0; i < sizeof(v) / sizeof(*v); ++i) {
		v[i] = v[i] + rhs.v[i] ;
	}
	
	return *this ;
}

Matrix4& Matrix4::operator-=(const Matrix4& rhs) {
	for (u32 i = 0; i < sizeof(v) / sizeof(*v); ++i) {
		v[i] = v[i] - rhs.v[i] ;
	}
	return *this ;
}

Matrix4& Matrix4::operator*=(const Matrix4& rhs) {
	*this = *this * rhs ;
	return *this ;
}

Matrix4 Matrix4::operator*(const f32 rhs) const {
	Matrix4 mat(*this) ;
	for (f32 &f : mat.v)
		f *= rhs ;
	return mat ;
}

Matrix4 Matrix4::operator/(const f32 rhs) const {
	Matrix4 mat(*this) ;
	for (f32 &f : mat.v)
		f /= rhs ;
	return mat ;
}

Matrix4& Matrix4::operator*=(const f32 rhs) {
	for (f32 &f : v)
		f *= rhs ;
	return *this ;
}

Matrix4& Matrix4::operator/=(const f32 rhs) {
	for (f32 &f : v)
		f /= rhs ;
	return *this ;
}

bool Matrix4::operator==(const Matrix4& rhs) const {
	for (u32 i = 0; i < sizeof(v) / sizeof(*v); ++i) {
		if (std::abs(v[i] - rhs.v[i]) > EPSILON)
			return false ;
	}

	return true ;
}

bool Matrix4::operator!=(const Matrix4& rhs) const {
	return !(*this == rhs) ;
}

// Zeroes out the entire matrix
void Matrix4::Zero(void) {
	for (f32 &f : v)
		f = 0 ;
}
  
// Builds the identity matrix
void Matrix4::Identity(void) {
	for (u32 i = 0; i < 4; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			m[i][j] = (i == j) ;
		}
	}
}

void Matrix4::Print(void) const {
	printf("--------------------------\n");
	printf("%5.3f %5.3f %5.3f %5.3f\n", m00, m01, m02, m03 );
	printf("%5.3f %5.3f %5.3f %5.3f\n", m10, m11, m12, m13 );
	printf("%5.3f %5.3f %5.3f %5.3f\n", m20, m21, m22, m23 );
	printf("%5.3f %5.3f %5.3f %5.3f\n", m30, m31, m32, m33 );
	printf("--------------------------\n");
}
