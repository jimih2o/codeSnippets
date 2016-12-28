/* Start Header -------------------------------------------------------
File Name: Matrix4.h
Purpose: Contains declaration for Matrix4
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/10/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "../Utilities.h"
#include "../type/Type.hpp"
////////////////////////////////////////////////////////////////////////////////
// DO NOT MODIFY THIS FILE FOR ASSIGNMENT 1
////////////////////////////////////////////////////////////////////////////////

#include "Point4.h"
#include "Vector4.h"

struct Matrix4
{
	union
	{
		struct  
		{
			f32 m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33;
		};

		f32 m[4][4];
		f32 v[16];
	};

	/* 
	This union lets us access the data in multiple ways
	All of these are modifying the same location in memory

	Matrix4 mtx;
	mtx.m[2][2] = 1.0f;
	mtx.v[10] = 2.0f;
	mtx.m22 = 3.0f;
	*/

	// Default constructor should initialize to zeroes
	Matrix4(void);

	// Copy constructor, copies every entry from the other matrix.
	Matrix4(const Matrix4& rhs);

	// Non-default constructor, self-explanatory
	Matrix4(f32 mm00, f32 mm01, f32 mm02, f32 mm03,
		f32 mm10, f32 mm11, f32 mm12, f32 mm13,
		f32 mm20, f32 mm21, f32 mm22, f32 mm23,
		f32 mm30, f32 mm31, f32 mm32, f32 mm33);

	// Assignment operator, does not need to handle self-assignment
	Matrix4& operator=(const Matrix4& rhs);

	// Multiplying a Matrix4 with a Vector4 or a Point4
	Vector4 operator*(const Vector4& rhs) const;
	Point4  operator*(const Point4& rhs) const;

	// Basic Matrix arithmetic operations
	Matrix4 operator+(const Matrix4& rhs) const;
	Matrix4 operator-(const Matrix4& rhs) const;
	Matrix4 operator*(const Matrix4& rhs) const;

	// Similar to the three above except they modify
	// the original 
	Matrix4& operator+=(const Matrix4& rhs);
	Matrix4& operator-=(const Matrix4& rhs);
	Matrix4& operator*=(const Matrix4& rhs);

	// Scale/Divide the entire matrix by a float
	Matrix4 operator*(const f32 rhs) const;
	Matrix4 operator/(const f32 rhs) const;

	// Same as previous 
	Matrix4& operator*=(const f32 rhs);
	Matrix4& operator/=(const f32 rhs);

	// Comparison operators which should use an epsilon defined in 
	// Utilities.h to see if the value is within a certain range
	// in which case we say they are equivalent.
	bool operator==(const Matrix4& rhs) const;
	bool operator!=(const Matrix4& rhs) const;

	// Zeroes out the entire matrix
	void Zero(void);

	// Builds the identity matrix
	void Identity(void);

	// Already implemented, simple print function
	void Print(void) const;

	static Matrix4 Translate(const Point4 &p) {
		return Matrix4(1, 0, 0, p.x,
					   0, 1, 0, p.y,
					   0, 0, 1, p.z,
					   0, 0, 0, 1) ;
	}

	static Matrix4 RotateX(float32 rad) {
		return Matrix4(1,             0,              0, 0,
					   0, std::cos(rad), -std::sin(rad), 0,
					   0, std::sin(rad),  std::cos(rad), 0,
					   0,             0,              0, 1) ; 
	}

	static Matrix4 RotateY(float32 rad) {
		return Matrix4(std::cos(rad),  0, std::sin(rad), 0,
					   0,              1,             0, 0,
					   -std::sin(rad), 0, std::cos(rad), 0,
					   0,              0,             0, 1) ;
	}

	static Matrix4 RotateZ(float32 rad) {
		return Matrix4(std::cos(rad), -std::sin(rad), 0, 0,
					   std::sin(rad),  std::cos(rad), 0, 0,
					   0,              0,             1, 0,
					   0,              0,             0, 1) ;
	}

	static Matrix4 Scale(float32 x, float32 y, float32 z) {
		return Matrix4(x, 0, 0, 0,
					   0, y, 0, 0,
					   0, 0, z, 0,
					   0, 0, 0, 1) ;
	}

	static Matrix4 RotateAxis(float32 theta, Vector4 const &axis) {
		float32 cos_theta = std::cos(theta) ;
		float32 sin_theta = std::sin(theta) ;
		Matrix4 s1, s2, s3 ;
		Vector4 n = axis ;
		n.Normalize() ;

		// scale matrix (contains the "1" for last element)
		s1 = Matrix4::Scale(cos_theta, cos_theta, cos_theta) ;
		
		// scaled tensor matrix
		s2 = Matrix4(n.x*n.x, n.y*n.x, n.z*n.x, 0,
					 n.x*n.y, n.y*n.y, n.z*n.y, 0,
					 n.x*n.z, n.y*n.z, n.z*n.z, 0,
					       0,       0,       0, 0) * (1.0F - cos_theta) ;

		// scaled cross matrix
		s3 = Matrix4(   0, -n.z,  n.y, 0,
			          n.z,    0, -n.x, 0,
			         -n.y,  n.x,    0, 0,
			            0,    0,    0, 0) * sin_theta ;

		return s1 + s2 + s3 ;
	}
	
	static Matrix4 Scale(const Point4 &s) {
		return Scale(s.x, s.y, s.z) ;
	}

	static Matrix4 IDENTITY() {
		Matrix4 m ;
		m.Identity() ;
		return m ;
	}
};
