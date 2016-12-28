#pragma once

//#include <intrin.h>
#include <fvec.h>

namespace Intrinsic
{
	typedef F32vec4 Vec4 ;
	struct Mat4 {
		F32vec4 m[4] ;
		inline F32vec4 &operator[](int row) {
			return m[row] ;
		}

		inline F32vec4 operator[](int row) const {
			return m[row] ;
		}
	};
	enum {X = 3, Y = 2, Z = 1, W = 0};

	Vec4 operator*(Mat4 const &m, Vec4 const &v) ;
}
