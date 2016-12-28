/* Start Header -------------------------------------------------------
File Name: Vertex.hpp
Purpose: Contains definition for Vertex
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
	struct Vertex {
		Vec4 position ;
		Vec4 normal ;
		Vec4 color ;
		Vec2 texCoord ;

		float32 cz ;

		Vertex() :
			position(0,0,0,1), color(1, 1, 1, 1), texCoord(0, 0), cz(0) {}

		Vertex(const Vec4 &pos, const Vec4 &col = Vec4(1, 1, 1, 1), const Vec2 &uv = Vec2(0, 0)) 
			: position(pos), color(col), texCoord(uv),cz(0) {}

		bool operator==(Vertex const &other) const {
			return 
				math::LengthSq(position - other.position) < math::Epsilon
			 && math::LengthSq(  normal -   other.normal) < math::Epsilon 
			 && math::LengthSq(   color -    other.color) < math::Epsilon
			 && math::LengthSq(texCoord - other.texCoord) < math::Epsilon ;
		}

		Vertex operator+(Vertex const &other) const {
			Vertex n ;
			n.position = position + other.position ;
			n.normal   = normal   + other.normal ;
			n.color    = color    + other.color ;
			n.texCoord = texCoord + other.texCoord ;
			n.cz = cz + other.cz ;
			return n ;
		}

		Vertex operator-(Vertex const &other) const {
			Vertex n ;
			n.position = position - other.position ;
			n.normal   = normal   - other.normal ;
			n.color    = color    - other.color ;
			n.texCoord = texCoord - other.texCoord ;
			n.cz = cz + other.cz ;
			return n ;
		}

		Vertex operator*(float t) const {
			Vertex n ;
			n.position = position * t ;
			n.normal   = normal   * t ;
			n.color    = color    * t ;
			n.texCoord = texCoord * t ;
			n.cz = cz * t ;
			return n ;
		}
	};
}

static graphics::Vertex operator*(float t, graphics::Vertex const &v) {
	return v * t ;
}