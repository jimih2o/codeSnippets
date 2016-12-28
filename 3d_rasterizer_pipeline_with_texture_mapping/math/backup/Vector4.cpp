/* Start Header -------------------------------------------------------
File Name: Vector4.cpp
Purpose: Contains definition for Vector4
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_1
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/10/2013
- End Header --------------------------------------------------------*/
#include "Vector4.h"

#include <cmath>

Vector4::Vector4(void)
: x(0), y(0), z(0), w(0) {

}
  
// Copy constructor, copies every component from the other Vector4
Vector4::Vector4(const Vector4& rhs) 
: x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {
	
}
  
// Non-Default constructor, self explanatory
Vector4::Vector4(f32 xx, f32 yy, f32 zz, f32 ww) 
: x(xx), y(yy), z(zz), w(ww) {
}
  
// Assignment operator, does not need to handle self assignment
Vector4& Vector4::operator=(const Vector4& rhs) {
	x = rhs.x ;
	y = rhs.y ;
	z = rhs.z ;
	w = rhs.w ;
	return *this ;
}
  
// Unary negation operator, negates all components and returns a copy
Vector4 Vector4::operator-(void) const {
	return Vector4(-x, -y, -z, -w) ;
}
	
// Basic Vector math operations. Add Vector to Vector B, or Subtract Vector A 
// from Vector B, or multiply a vector with a scalar, or divide a vector by 
// that scalar
Vector4 Vector4::operator+(const Vector4& rhs) const {
	return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w) ;
}

Vector4 Vector4::operator-(const Vector4& rhs) const {
	return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w) ;
}

Vector4 Vector4::operator*(const f32 rhs) const {
	return Vector4(x * rhs, y * rhs, z * rhs, w * rhs) ;
}

Vector4 Vector4::operator/(const f32 rhs) const {
	return Vector4(x / rhs, y / rhs, z / rhs, w / rhs) ;
}
  
// Same as above, just stores the result in the original vector
Vector4& Vector4::operator+=(const Vector4& rhs) {
	x += rhs.x ;
	y += rhs.y ;
	z += rhs.z ;
	w += rhs.w ;
	return *this ;
}

Vector4& Vector4::operator-=(const Vector4& rhs) {
	x -= rhs.x ;
	y -= rhs.y ;
	z -= rhs.z ;
	w -= rhs.w ;
	return *this ;
}

Vector4& Vector4::operator*=(const f32 rhs) {
	x *= rhs ;
	y *= rhs ;
	z *= rhs ;
	w *= rhs ;
	return *this ;
}

Vector4& Vector4::operator/=(const f32 rhs) {
	x /= rhs ;
	y /= rhs ;
	z /= rhs ;
	w /= rhs ;
	return *this ;
}
  
// Comparison operators which should use an epsilon defined in 
// Utilities.h to see if the value is within a certain range
// in which case we say they are equivalent.
bool Vector4::operator==(const Vector4& rhs) const {
	return std::abs(x - rhs.x) < EPSILON 
		&& std::abs(y - rhs.y) < EPSILON 
		&& std::abs(z - rhs.z) < EPSILON 
		&& std::abs(w - rhs.w) < EPSILON ;		
}

bool Vector4::operator!=(const Vector4& rhs) const {
	return !(*this == rhs) ;
}
  
// Computes the dot product with the other vector. Treat it as 3D vector.
f32 Vector4::Dot(const Vector4& rhs) const {
	return (x * rhs.x) + (y * rhs.y) + (z * rhs.z) ;
}
  
// Computes the cross product with the other vector. Treat it as a 3D vector.
Vector4 Vector4::Cross(const Vector4& rhs) const {
	return Vector4(y * rhs.z - z * rhs.y,
				   z * rhs.x - x * rhs.z,
				   x * rhs.y - y * rhs.x) ;
}
  
// Computes the true length of the vector 
f32 Vector4::Length(void) const {
	return std::sqrt(LengthSq()) ;
}
  
// Computes the squared length of the vector
f32 Vector4::LengthSq(void) const {
	return x * x + y * y + z * z + w * w ;
}
  
// Normalizes the vector to make the final vector be of length 1. If the length is zero
// then this function should not modify anything.
void Vector4::Normalize(void) {
	f32 len = Length() ;
	if (len > EPSILON)
		*this /= len ;
}
  
// Sets x,y,z to zeroes, w to defined value
void Vector4::Zero(void) {
	x = y = z = w = 0 ;
}

void Vector4::Print(void) const {
	printf("%5.3f, %5.3f, %5.3f, %5.3f\n", x, y, z, w) ;
}
