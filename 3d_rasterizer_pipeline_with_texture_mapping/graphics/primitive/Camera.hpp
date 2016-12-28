/* Start Header -------------------------------------------------------
File Name: Camera.hpp
Purpose: Contains declaration for Camera
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "../../type/Type.hpp"
#include "../../math/MathDef.hpp"

namespace graphics {
	class Camera {
	public:
		float32 yFOV, r,l,t,b, focal, near, far ;
		Vec4 position ;
		Vec4 view ;
		Vec4 up ;
		Vec4 right ;

		Camera() ;
		Mat4 GetTransform() ;
	};
}