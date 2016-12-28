/* Start Header -------------------------------------------------------
File Name: Tank.cpp
Purpose: Contains definition for Tank
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#include "Tank.hpp"

#include "graphics\base\Rasterizer.hpp"

void TankModel::Draw(graphics::Rasterizer &context) {
	context.stack.Push(math::Translate(body.position)) ;
	context.stack.Push(math::RotateY(body.rotation.y)) ;

	// draw body
	context.stack.Push(Matrix4::Scale(body.scale)) ;
	context.Draw(mesh, body.r, body.g, body.b) ;
	context.stack.Pop() ;

	context.stack.Push(Matrix4::Translate(turret.position)) ;
	context.stack.Push(Matrix4::RotateY(turret.rotation.y)) ;

	// draw turret
	context.stack.Push(Matrix4::Scale(turret.scale)) ;
	context.Draw(mesh, turret.r, turret.g, turret.b) ;
	context.stack.Pop() ;


	context.stack.Push(Matrix4::RotateX(gun.rotation.x)) ;
	context.stack.Push(Matrix4::Translate(gun.position)) ;
	context.stack.Push(Matrix4::Scale(gun.scale)) ;

	// draw gun
	context.Draw(mesh, gun.r, gun.g, gun.b) ;

	context.stack.Pop(5) ;

	// draw wheels
	for (auto wheel : wheels) {
		context.stack.Push(Matrix4::Translate(wheel.position)) ;
		context.stack.Push(Matrix4::RotateX(wheel.rotation.x)) ;
		context.stack.Push(Matrix4::Scale(wheel.scale)) ;
		context.Draw(mesh, wheel.r, wheel.g, wheel.b) ;
		context.stack.Pop(3) ;
	}

	context.stack.Pop(2) ;
}
