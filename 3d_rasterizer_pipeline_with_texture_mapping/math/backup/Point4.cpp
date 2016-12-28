/* Start Header -------------------------------------------------------
File Name: Point4.cpp
Purpose: Contains definition for Point4
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/10/2013
- End Header --------------------------------------------------------*/
#include "Point4.h"
#include "Vector4.h"

#include <cmath>

Point4::Point4(void)
: x(0), y(0), z(0), w(1) {

}

Point4::Point4(const Point4& rhs) 
: x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {
	
}

Point4::Point4(f32 xx, f32 yy, f32 zz, f32 ww) 
: x(xx), y(yy), z(zz), w(ww) {
}

Point4& Point4::operator=(const Point4& rhs) {
	x = rhs.x ;
	y = rhs.y ;
	z = rhs.z ;
	w = rhs.w ;
	return *this ;
}

Vector4 Point4::operator-(const Point4& rhs) const {
	return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w) ;
}

Point4 Point4::operator-(void) const {
	return Point4(-x, -y, -z, -w) ;
}

Point4 Point4::operator+(const Vector4& rhs) const {
	return Point4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w) ;
}

Point4 Point4::operator-(const Vector4& rhs) const {
	return Point4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w) ;
}

Point4& Point4::operator+=(const Vector4& rhs) {
	x += rhs.x ;
	y += rhs.y ;
	z += rhs.z ;
	w += rhs.w ;
	return *this ;
}

Point4& Point4::operator-=(const Vector4& rhs) {
	x -= rhs.x ;
	y -= rhs.y ;
	z -= rhs.z ;
	w -= rhs.w ;
	return *this ;
}

bool Point4::operator==(const Point4& rhs) const {
	return std::abs(x - rhs.x) < EPSILON 
		&& std::abs(y - rhs.y) < EPSILON 
		&& std::abs(z - rhs.z) < EPSILON 
		&& std::abs(w - rhs.w) < EPSILON ;		
}

bool Point4::operator!=(const Point4& rhs) const {
	return !(*this == rhs) ;
}
  
// Sets x,y,z to zeroes, w to defined value
void Point4::Zero(void) {
	x = y = z = 0 ;
	w = 1 ;
}

void Point4::Print(void) const {
  printf("%5.3f, %5.3f, %5.3f, %5.3f\n", x, y, z, w) ;
}
