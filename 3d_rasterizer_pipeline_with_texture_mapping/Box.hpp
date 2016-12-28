/* Start Header -------------------------------------------------------
File Name: Box.hpp
Purpose: Create boxes
Language: C++
Platform: Windows
Project: jimi.e_CS250_3
Author: Jimi Huard, jimi.e 180003312
Creation date: 10/1/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "graphics/base/Model.hpp"
#include "graphics/base/Rasterizer.hpp"

class Box : public graphics::Model {
public:
	graphics::Mesh mesh ;
	graphics::Transform transform ;

	virtual void Draw(graphics::Rasterizer &Graphics) {
		Graphics.stack.Push(math::Translate(transform.position)) ;
		Graphics.stack.Push(math::Scale4D(transform.scale)) ;
		Graphics.Draw(mesh) ;
		Graphics.stack.Pop(2) ;
	}
};