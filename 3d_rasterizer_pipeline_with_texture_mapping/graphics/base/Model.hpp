/* Start Header -------------------------------------------------------
File Name: Model.hpp
Purpose: Contains declaration for model
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "../../type/Type.hpp"
#include "../../type/Array.hpp"
#include "../primitive/Vertex.hpp"

namespace graphics {
	class Rasterizer ;

	struct Mesh {
		Array<Vertex> verts ;
		Array<uint32> inds ;
	};

	struct Transform {
		Vec3 position ;
		Vec4 scale ;
		Vec4 right, up, direction ;

		Transform() : scale(1,1,1,1), right(1,0,0,0), up(0,1,0,0), direction(0,0,1,0) {};

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
	};

	class Model {
	public:
		virtual void Draw(Rasterizer &context) = 0 ;
	};
}