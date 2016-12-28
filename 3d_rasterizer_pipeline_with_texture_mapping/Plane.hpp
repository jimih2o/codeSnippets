/* Start Header -------------------------------------------------------
File Name: Plane.hpp
Purpose: Contains declaration for Plane
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_4
Author: Jimi Huard, jimi.e 180003312
Creation date: 10/17/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "graphics/base/Model.hpp"
#include "graphics/base/Rasterizer.hpp"

class PlaneModel : public graphics::Model {
public:
	graphics::Mesh mesh ;
	graphics::Transform body, tail, leftWing, rightWing ;
	Vec4 right, up, direction ;
	float32 speed ;

	PlaneModel() {
		right     = Vec4(1, 0, 0) ;
		up        = Vec4(0, 1, 0) ;
		direction = Vec4(0, 0, 1) ;
		speed     = 10 ;
	}

	void Yaw(float32 theta) {
		Mat4 rot = math::Rotate4D(theta, up) ;
		right     = rot * right ;
		direction = rot * direction ;
	}

	void Pitch(float32 theta) {
		Mat4 rot = math::Rotate4D(theta, right) ;
		up        = rot * up ;
		direction = rot * direction ;
	}

	void Roll(float32 theta) {
		Mat4 rot = math::Rotate4D(theta, direction) ;
		right = rot * right ;
		up    = rot * up ;
	}

	virtual void Draw(graphics::Rasterizer &context) {
		context.stack.Push(math::Translate(body.position)) ;
		context.stack.Push(Mat4(
			right.x, up.x, direction.x, 0,
			right.y, up.y, direction.y, 0,
			right.z, up.z, direction.z, 0,
			      0,    0,           0, 1
			)) ;
		
		context.stack.Push(math::Scale4D(body.scale)) ;
		context.Draw(mesh) ;
		context.stack.Pop() ;

		context.stack.Push(math::Translate(tail.position)) ;
		context.stack.Push(math::Scale4D(tail.scale)) ;
		context.Draw(mesh) ;
		context.stack.Pop(2) ;

		context.stack.Push(math::Translate(leftWing.position)) ;
		context.stack.Push(math::Scale4D(leftWing.scale)) ;
		context.Draw(mesh) ;
		context.stack.Pop(2) ;

		context.stack.Push(math::Translate(rightWing.position)) ;
		context.stack.Push(math::Scale4D(rightWing.scale)) ;
		context.Draw(mesh) ;
		context.stack.Pop(2) ;

		context.stack.Pop(2) ;
	}
};