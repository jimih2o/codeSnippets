/* Start Header -------------------------------------------------------
File Name: Transform.hpp
Purpose: Contains definition for Transform
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "../../math/Math.hpp"

struct Transform {
	Point4 position ;
	Point4 rotation ;
	Point4 scale ;
	byte r, g, b ;

	Transform() : scale(1, 1, 1), r(0), g(0), b(0) {}

	Matrix4 Compute() const {
		return Matrix4::Translate(position) * 
			Matrix4::RotateX(rotation.x) * Matrix4::RotateY(rotation.y) * Matrix4::RotateZ(rotation.z) * 
			Matrix4::Scale(scale.x, scale.y, scale.z) ;
	}
};
