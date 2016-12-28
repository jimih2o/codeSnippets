/* Start Header -------------------------------------------------------
File Name: Tank.hpp
Purpose: Contains declaration for Tank
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "graphics/base/Model.hpp"

class TankModel : public graphics::Model {
public:
	graphics::Mesh mesh ;
	Transform body, wheels[4], turret, gun ;
	virtual void Draw(graphics::Rasterizer &context) ;
};