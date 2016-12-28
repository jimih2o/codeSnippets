/* Start Header -------------------------------------------------------
File Name: Camera.cpp
Purpose: Contains definition for Camera
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#include "Camera.hpp"

namespace graphics {
	Camera::Camera() 
	: yFOV(90), focal(5), near(0), far(0) {
		view = Vec4(0, 0, -1) ;
		up = Vec4(0, 1, 0) ;
		right = Vec4(1,0,0) ;
	}

	Mat4 Camera::GetTransform() {
		math::Normalize(view) ;
		math::Normalize(up) ;
		math::Normalize(right) ;

		return 
			Mat4(
			right.x, right.y, right.z, 0,
			up.x,    up.y,    up.z, 0,
			-view.x, -view.y, -view.z, 0,
			0,       0,       0, 1
			) * math::Translate(-position.xyz()) ;
	}
}